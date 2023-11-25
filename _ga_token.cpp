/* 
 * File:   _ga_token.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 08:51
 */

#include "_ga_token.h"
#include "_ga_agent.h"
#include "_stepPath.h"
#include "_task.h"
#include "_closerEndpointIndexerAlgorithm.h"

_ga_token::_ga_token(
        const _map& map,
        const _stepMap& stepMap,
        const _agent_energy_system& agent_energy_system) :
map(map),
stepMap(stepMap),
agent_energy_system(agent_energy_system) {

    map.listBotsEndPoints([this, agent_energy_system](unsigned botId, const _site & endpoint) {

        agents.insert(
                std::pair<unsigned, _ga_agent>(
                botId,
                _ga_agent(
                botId,
                _stepSite(0, endpoint.GetRow(), endpoint.GetColunm()),
                agent_energy_system)));

        return false;

    });

    map.listEndpoints([this, map](const _site & endpoint) {

        nonTaskEndpoints.insert(std::pair<unsigned, _site>(
                endpoint.linearIndex(map.getColumn_size()),
                endpoint));

        return false;

    });



}

_ga_token::_ga_token(const _ga_token& other) :
map(other.map),
stepMap(other.stepMap),
nonTaskEndpoints(other.nonTaskEndpoints),
pendingTasks(other.pendingTasks),
assignedTasks(other.assignedTasks),
runningTasks(other.runningTasks),
finishedTasks(other.finishedTasks),
agents(other.agents),
assignTaskAgent(other.assignTaskAgent),
agent_energy_system(other.agent_energy_system),
currentStep(other.currentStep) {
}

_ga_token::~_ga_token() {
}

void _ga_token::draw(const Render& render) const {

    for (const auto& pagent : agents) {

        pagent.second.draw(render);

    }

}

const _ga_agent& _ga_token::getAgentById(int agentId)const {

    std::map<int, _ga_agent>::const_iterator it = agents.find(agentId);

    if (it == agents.end()) {

        try {
            std::ostringstream stream;
            stream << "agent id not found: " << agentId;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return it->second;

}

void _ga_token::writeHeader(std::ostream& fs) const {
    Writable::strWrite(*this, fs, "token_id", true);
    Writable::strWrite(*this, fs, "token_name", true);
    Writable::strWrite(*this, fs, "current_step", true);
    Writable::strWrite(*this, fs, "energy system id", true);
    Writable::strWrite(*this, fs, "energy expenditure", true);
    Writable::strWrite(*this, fs, "finished_tasks", false);
}

void _ga_token::writeRow(std::ostream& fs) const {
    Writable::strWrite(*this, fs, id(), true);
    Writable::strWrite(*this, fs, getName(), true);
    Writable::uintWrite(*this, fs, currentStep, true);
    Writable::strWrite(*this, fs, agent_energy_system.id(), true);
    Writable::intWrite(*this, fs, energyExpenditure(), true);
    Writable::uintWrite(*this, fs, finishedTasks.size(), false);
}

unsigned _ga_token::getCurrentStep() const {
    return currentStep;
}

void _ga_token::stepping() {

//        if(currentStep > 38){
//            stepMap.stepView(currentStep);
//            stepMap.free_step_view();
//            stepMap.free_agent_view();
//            std::cout << std::endl;
//        }

    for (auto &pagent : agents) {

        pagent.second.stepping(map);

    }

    currentStep++;

}

const _map& _ga_token::getMap() const {
    return map;
}

const _stepMap& _ga_token::getStepMap() const {
    return stepMap;
}

unsigned _ga_token::numberOfpendingTasks()const {
    return pendingTasks.size();
}

unsigned _ga_token::getRunningTaskAmount()const {
    return runningTasks.size();
}

unsigned _ga_token::getAssignedTaskAmount()const {
    return assignedTasks.size();
}

unsigned _ga_token::getPendingTaskAmount()const {
    return pendingTasks.size();
}

unsigned _ga_token::getFinishedTaskAmount()const {
    return finishedTasks.size();
}

const _agent_energy_system& _ga_token::getAgent_energy_system() const {
    return agent_energy_system;
}

unsigned _ga_token::numberOfAgents()const {
    return agents.size();
}

_ga_token* _ga_token::getClone()const {
        return new _ga_token(*this);
}

std::string _ga_token::id() const {
    return "GAT";
}

const std::string& _ga_token::getName() const {
    return _name;
}

void _ga_token::setName(const std::string& name){
        this->_name = name;
    }

int _ga_token::energyExpenditure() const {
    int ret = 0;
    listConstAgents([&ret](unsigned index, const _ga_agent & agent) {
        ret += (agent.getAgentEnergySystem().getCharging() - agent.getAgentEnergySystem().currentLevel());
        return false;
    });
    return ret;
}

bool _ga_token::isAgentDelivering(int agentId) const {
    return getAgentById(agentId).isGoingToDelivering();
}

bool _ga_token::isAgentPickuping(int agentId) const {
    return getAgentById(agentId).isGoingToPickuping();
}

bool _ga_token::isAgentAtTrivialPath(int agentId) const {
    return getAgentById(agentId).isAtTrivialPath();
}

bool _ga_token::isIdle()const {

    return pendingTasks.empty() && runningTasks.empty();

}

bool _ga_token::isLate()const {

    return !pendingTasks.empty();

}

bool _ga_token::liberateEndpoint(const _ga_agent& agent, const _site& endpoint) {

    _ga_agent* obstructor = getEndpointObstructor(agent, endpoint);

    if (obstructor != nullptr) {

//        updateAgentRestPathCloserEndpoint(*obstructor, obstructor->currentSite());
        updateAgentRestPathCloserEndpoint(*obstructor, endpoint);

        return true;

    }

    return false;

}

_ga_agent* _ga_token::getEndpointObstructor(const _ga_agent& agent, const _site& endpoint) {

    for (auto &agentPair : agents) {

        if (agent.id() != agentPair.second.id() && agentPair.second.goalSite().match(endpoint)) { //other agents

            return &agentPair.second;

        }

    }

    return nullptr;

}

bool _ga_token::updateAgentRestPathCloserEndpoint(_ga_agent& agent, const _site& closerEndpoint) {

    _stepAstarAlgorithm astar;

    _closerEndpointIndexerAlgorithm closerEndpointIndexerAlgorithm;

    std::vector<_site> vsite;

    listNonTaskEndpoints([&closerEndpointIndexerAlgorithm, &vsite, &agent, &closerEndpoint, this](const _site & endpoint) {

        closerEndpointIndexerAlgorithm.ga_solve_to_site(*this, endpoint, closerEndpoint, vsite);

        return false;

    });

    for (auto& endpoint : vsite) {

        bool flag = true;

        listConstAgents([&endpoint, &agent, &flag](unsigned index, const _ga_agent & otherAgent) {

            if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents

                flag = false;

                return true;

            }

            return false;

        });

        if (flag) {

            _stepPath newAgentPath(agent.goalSite());
            bool flag = astar.solve(stepMap, newAgentPath, endpoint, agent.id());

            if (flag) {

                if (newAgentPath.isTrivial()) {

//                    _stepSite future = agent.currentSite();
//                    future.SetStep(future.GetStep() + 1);
//                    newAgentPath.progress(future);
                    
                    try {
                        std::ostringstream stream;
                        stream << "trivial path to rest endpoint";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }
                    

                }


                stepMap.setMoving(newAgentPath, agent.id());
                newAgentPath.pop();
                agent.progressPath(newAgentPath);

                return true;

            } else {

                try {
                    std::ostringstream stream;
                    stream << "no task endpoint site path not found" << endpoint;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        }

    }

    try {
        std::ostringstream stream;
        stream << "no endpoint available";
        MAPD_EXCEPTION(stream.str());
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::abort();
    }

    return false;

}

bool _ga_token::updateAgentTaskPath_pendingTask(_ga_agent& agent, int newTaskId, bool trivialPath){
    
    bool ret = false;
    
    _stepPath newAgentPath(agent.currentSite());
    
    agent.unassignTask();
    agent.setStateFree();
    
    std::map<int, _task>::const_iterator new_task_it = pendingTasks.find(newTaskId);

    if (new_task_it != pendingTasks.end()) {

        const _task pendingTask = new_task_it->second;
        
        if (pendingTask.getPickup().match(pendingTask.getDelivery())) { // innocuous pending task   

            finishedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
            pendingTasks.erase(new_task_it);
            assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));
            
            ret = true;
            
            if(!trivialPath){
                
                return true;
                
            }

        } else { // go to pickup   
            
            if(!trivialPath) stepMap.deleteMoving(agent.getPath(), agent.id());
            
            agent.assignTask(pendingTask);

            if (agent.currentSite().match(pendingTask.getPickup())) {

                runningTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
                pendingTasks.erase(new_task_it);
                assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));
                
                agent.setStateBuzy();

                ret = true;

                liberateEndpoint(agent, pendingTask.getDelivery());

                _stepAstarAlgorithm astar;

                bool flag = astar.solve(stepMap, newAgentPath, pendingTask.getDelivery(), agent.id());

                if (!flag) {

                    try {
                        std::ostringstream stream;
                        stream << "delivery site path not found: " << pendingTask.getDelivery();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;                        
                        stepMap.stepView(currentStep);
                        std::abort();
                    }

                }

            } else {

                liberateEndpoint(agent, pendingTask.getPickup());

                _stepAstarAlgorithm astar;

                bool flag = astar.solve(stepMap, newAgentPath, pendingTask.getPickup(), agent.id());

                if (!flag) {
                    
                    for (int i = std::max<int>((int)currentStep - 10 ,0); i < currentStep; i++) {
                        
                        stepMap.stepView(i);                        

                    }
                    
                    std::cout << "////////////////" << std::endl;
                    
                    std::cout << "current step:" << std::endl;
                    
                    stepMap.stepView(currentStep);
                    
                    stepMap.free_agent_view();
                    stepMap.max_step_view();
                    
                    std::cout << "////////////////" << std::endl;
                    
                    for (int i = currentStep + 1; i < currentStep + 10; i++) {
                        
                        stepMap.stepView(i);                        

                    }
                    

                    try {
                        std::ostringstream stream;
                        stream << "pickup site path not found: " << pendingTask.getPickup();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            }      
            
        }
        
        
        
    } else {
        
        if(!trivialPath){

            return false;

        }
                
    }
    
    if (newAgentPath.isTrivial()) {

        _stepSite future = agent.currentSite();
        future.SetStep(future.GetStep() + 1);
        newAgentPath.progress(future);

    }

    agent.assignPath(newAgentPath);
    stepMap.setMoving(newAgentPath, agent.id());
    
    return ret;
    
}

bool _ga_token::updateAgentTaskPath_resting(_ga_agent& agent, int newTaskId) {
    
    return updateAgentTaskPath_pendingTask(agent, newTaskId, true);

}

bool _ga_token::updateAgentTaskPath_going_to_resting(_ga_agent& agent, int newTaskId) {
    
    return updateAgentTaskPath_pendingTask(agent, newTaskId, false);
    
}

bool _ga_token::updateAgentTaskPath_pickuping(_ga_agent& agent, int newTaskId) {

//    bool ret = false;

    _stepPath newAgentPath(agent.currentSite());

    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    if (currentTask->id() == newTaskId) {

        pendingTasks.erase(currentTask->id());
        runningTasks.insert(std::pair<int, _task>(currentTask->id(), *currentTask));
        assignTaskAgent.insert(std::pair<int, int>(currentTask->id(), agent.id()));

        agent.setStateBuzy();

//        ret = true;

        liberateEndpoint(agent, currentTask->getDelivery());

        _stepAstarAlgorithm astar;

        bool flag = astar.solve(stepMap, newAgentPath, currentTask->getDelivery(), agent.id());

        if (!flag) {

            try {
                std::ostringstream stream;
                stream << "delivery site path not found: " << currentTask->getDelivery();
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else { // different tasks
        
        
        return updateAgentTaskPath_pendingTask(agent, newTaskId, true);


    }

    if (newAgentPath.isTrivial()) {

        _stepSite future = agent.currentSite();
        future.SetStep(future.GetStep() + 1);
        newAgentPath.progress(future);

    }

    agent.assignPath(newAgentPath);
    stepMap.setMoving(newAgentPath, agent.id());
    
    return true;

}

bool _ga_token::updateAgentTaskPath_rest_pickuping(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    _stepPath newAgentPath(agent.currentSite());

    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    if (currentTask->id() == newTaskId) {

        liberateEndpoint(agent, currentTask->getPickup());

        _stepAstarAlgorithm astar;

        bool flag = astar.solve(stepMap, newAgentPath, currentTask->getPickup(), agent.id());

        if (!flag) {

            try {
                std::ostringstream stream;
                stream << "delivery site path not found: " << currentTask->getPickup();
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else { // different tasks
        
        return updateAgentTaskPath_pendingTask(agent, newTaskId, true);

    }

    if (newAgentPath.isTrivial()) {

        _stepSite future = agent.currentSite();
        future.SetStep(future.GetStep() + 1);
        newAgentPath.progress(future);

    }

    agent.assignPath(newAgentPath);
    stepMap.setMoving(newAgentPath, agent.id());

    return ret;

}

bool _ga_token::updateAgentTaskPath_going_to_pickuping(_ga_agent& agent, int newTaskId) {

//    bool ret = false;

    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    if (newTaskId != currentTask->id()) {

        return updateAgentTaskPath_pendingTask(agent, newTaskId, false);

    }
    
    return false;

}

bool _ga_token::updateAgentTaskPath_going_to_rest_pickuping(_ga_agent& agent, int newTaskId) {

    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    if (newTaskId != currentTask->id()) {

        return updateAgentTaskPath_pendingTask(agent, newTaskId, false);

    }
    
    return false;

}

bool _ga_token::updateAgentTaskPath_going_to_rest_pickuping_to_pickuping(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    _stepAstarAlgorithm astar;

    _stepPath newAgentPath(agent.currentSite());

    if (newTaskId == currentTask->id()) {

        runningTasks.insert(std::pair<int, _task>(currentTask->id(), *currentTask));
        pendingTasks.erase(currentTask->id());
        assignTaskAgent.insert(std::pair<int, int>(currentTask->id(), agent.id()));

        agent.setStateBuzy();

        stepMap.deleteMoving(agent.getPath(), agent.id());

        ret = true;

        liberateEndpoint(agent, currentTask->getDelivery());

        bool flag = astar.solve(stepMap, newAgentPath, currentTask->getDelivery(), agent.id());

        if (!flag) {
            
            for (int i = std::max<int>((int)currentStep - 10 ,0); i < currentStep; i++) {
                        
                stepMap.stepView(i);                        

            }

            std::cout << "////////////////" << std::endl;

            std::cout << "current step:" << std::endl;

            stepMap.stepView(currentStep);

            std::cout << "////////////////" << std::endl;

            for (int i = currentStep + 1; i < currentStep + 10; i++) {

                stepMap.stepView(i);                        

            }

            stepMap.free_agent_view();

            try {
                std::ostringstream stream;
                stream << "delivery site path not found: " << currentTask->getDelivery();
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else {
        
        return updateAgentTaskPath_pendingTask(agent, newTaskId, false);

    }

    if (newAgentPath.isTrivial()) {

        _stepSite future = agent.currentSite();
        future.SetStep(future.GetStep() + 1);
        newAgentPath.progress(future);

    }

    agent.assignPath(newAgentPath);
    stepMap.setMoving(newAgentPath, agent.id());

    return ret;

}

bool _ga_token::updateAgentTaskPath_delivering(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(currentTask->id());

    if (currentTask_it != runningTasks.end()) {

        finishedTasks.insert(std::pair<int, _task>(currentTask_it->first, currentTask_it->second));
        runningTasks.erase(currentTask_it);
        
        agent.setStateFree();
        agent.unassignTask();
        
    } else {

        try {
            std::ostringstream stream;
            stream << "running task id not found: " << currentTask->id();
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }
    
    return updateAgentTaskPath_pendingTask(agent, newTaskId, true);

}

bool _ga_token::updateAgentTaskPath_rest_delivering(_ga_agent& agent) {

    _stepPath newAgentPath(agent.currentSite());
    
    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(currentTask->id());

    if (currentTask_it != runningTasks.end()) {

        liberateEndpoint(agent, currentTask->getDelivery());

        _stepAstarAlgorithm astar;

        bool flag = astar.solve(stepMap, newAgentPath, currentTask->getDelivery(), agent.id());

        if (!flag) {

            try {
                std::ostringstream stream;
                stream << "delivery site path not found" << currentTask->getDelivery();
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

        if (newAgentPath.isTrivial()) {

            _stepSite future = agent.currentSite();
            future.SetStep(future.GetStep() + 1);
            newAgentPath.progress(future);

        }

        agent.assignPath(newAgentPath);
        stepMap.setMoving(newAgentPath, agent.id());

    } else {

        try {
            std::ostringstream stream;
            stream << "running task id not found: " << currentTask->id();
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return false;

}

bool _ga_token::updateAgentTaskPath_going_to_rest_delivering_to_delivering(_ga_agent& agent, int newTaskId) {

    const _task* currentTask = agent.getCurrentTask();
    
    if(currentTask == nullptr){
        
        try {
            std::ostringstream stream;
            stream << "no task assigned";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(currentTask->id());

    if (currentTask_it != runningTasks.end()) {

        runningTasks.erase(currentTask_it);
        finishedTasks.insert(std::pair<int, _task>(currentTask->id(), *currentTask));
           
    } else {

        try {
            std::ostringstream stream;
            stream << "running task id not found: " << currentTask->id();
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }
    
    return updateAgentTaskPath_pendingTask(agent, newTaskId, false);

}

bool _ga_token::updateAgentTaskPath(
        int agentId,
        int newTaskId) {
    
//        if(currentStep >= 20)
//            stepMap.stepView(currentStep);
        
//    std::cout << "step: " << currentStep << " - ";

    std::map<int, _ga_agent>::iterator agent_it = agents.find(agentId);

    if (agent_it != agents.end()) {

        _ga_agent& agent = agent_it->second;
        
//        std::cout << "agent: " << agent.id() << " - ";

        if (agent.isAtTrivialPath()) {
            
//            std::cout << "trivial" << " - ";

            if (agent.isFree()) {

                if (agent.isAtPickuping()) {
                    
//                    std::cout << "agent.isAtPickuping() : "<< std::endl;

                    return updateAgentTaskPath_pickuping(agent, newTaskId);

                } else if (agent.isAtResting()) {
                    
//                    std::cout << "agent.isAtResting() : "<< std::endl;

                    return updateAgentTaskPath_resting(agent, newTaskId);

                } else if (agent.isAtRestPickuping()) {
                    
//                    std::cout << "agent.isAtRestPickuping() : "<< std::endl;

                    return updateAgentTaskPath_rest_pickuping(agent, newTaskId);

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "unrecognized state";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            } else if (agent.isBuzy()) {

                if (agent.isAtDelivering()) {
                    
//                    std::cout << "agent.isAtDelivering() : "<< std::endl;

                    return updateAgentTaskPath_delivering(agent, newTaskId);

                } else if (agent.isAtRestDelivering()) {
                    
//                    std::cout << "agent.isAtRestDelivering() : "<< std::endl;

                    return updateAgentTaskPath_rest_delivering(agent);

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "unrecognized state";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            } else {

                try {
                    std::ostringstream stream;
                    stream << "unrecognized state";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        } else { // !agent.isAtTrivialPath()
            
//            std::cout << "no trivial" << " - ";

            if (agent.isFree()) {

                if (agent.isAtPickuping()) {
                    
//                    std::cout << "*agent.isAtPickuping() : "<< std::endl;

                    return updateAgentTaskPath_going_to_rest_pickuping_to_pickuping(agent, newTaskId);

                } else if (agent.isGoingToPickuping()) {
                    
//                    std::cout << "*agent.isGoingToPickuping() : "<< std::endl;

                    return updateAgentTaskPath_going_to_pickuping(agent, newTaskId);

                } else if (agent.isGoingToRestPickuping()) {
                    
//                    std::cout << "*agent.isGoingToRestPickuping() : "<< std::endl;

                    return updateAgentTaskPath_going_to_rest_pickuping(agent, newTaskId);

                } else if (agent.isGoingToResting()) {
                    
//                    std::cout << "*agent.isGoingToResting() : "<< std::endl;

                    return updateAgentTaskPath_going_to_resting(agent, newTaskId);

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "unrecognized state";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            } else if (agent.isBuzy()) {

                if (agent.isGoingToDelivering()) {
                    
//                    std::cout << "*agent.isGoingToDelivering() : "<< std::endl;

                    // nothing

                } else if (agent.isAtDelivering()) {
                    
//                    std::cout << "*agent.isAtDelivering() : "<< std::endl;

                    return updateAgentTaskPath_going_to_rest_delivering_to_delivering(agent, newTaskId);

                } else if (agent.isGoingToRestDelivering()) {

//                    std::cout << "*agent.isGoingToRestDelivering() : "<< std::endl;
                    // nothing

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "unrecognized state";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            } else {

                try {
                    std::ostringstream stream;
                    stream << "unrecognized state";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        }

    } else {

        try {
            std::ostringstream stream;
            stream << "agent id not found: " << agentId;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return false;

}

void _ga_token::addPendingTask(const _task& task) {
    this->pendingTasks.insert(std::pair<unsigned, _task>(task.id(), task));
    this->nonTaskEndpoints.erase(task.getPickup().linearIndex(map.getColumn_size()));
    this->nonTaskEndpoints.erase(task.getDelivery().linearIndex(map.getColumn_size()));
}

void _ga_token::listNonTaskEndpoints(const std::function<bool(const _site&) > function)const {

    for (auto pendpoint : nonTaskEndpoints) {

        if (function(pendpoint.second))return;

    }

}

const _task* _ga_token::getPendingTaskById(int taskId) const {

    auto it = pendingTasks.find(taskId);

    if (it != pendingTasks.end()) {

        return &it->second;

    }

    return nullptr;

}

void _ga_token::listPendingTasks(const std::function<bool(unsigned, const _task&) > function)const {

    unsigned index = 0;

    for (auto &taskPair : pendingTasks) {

        if (function(index++, taskPair.second))return;

    }

}

void _ga_token::listAgents(const std::function<bool(_ga_agent&) > function) {

    for (auto &agentPair : agents) {

        if (function(agentPair.second))return;

    }

}

void _ga_token::listConstAgents(const std::function<bool(unsigned, const _ga_agent&) > function)const {

    unsigned index = 0;

    for (auto &agentPair : agents) {

        if (function(index++, agentPair.second))return;

    }

}

void _ga_token::error_site_collision_check() const {

    unsigned count1 = agents.size();

    for (const auto& pagent1 : agents) {

        unsigned count2 = count1;

        for (const auto& pagent2 : agents) {

            if (count2 > 0) {

                if (pagent1.second.id() != pagent2.second.id() &&
                        pagent1.second.currentSite().match(pagent2.second.currentSite())) {

                    try {
                        std::ostringstream stream;
                        stream << "site collision between agent " << pagent1.second.id();
                        stream << " and agent " << pagent2.second.id() << " step(" << currentStep << ")";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        stepMap.stepView(currentStep);
                        std::abort();
                    }

                }

                count2--;

            } else {

                break;

            }

        }

        count1--;

    }

}

unsigned _ga_token::GetMaxPlannedStep() const {

    unsigned maxStep = 0;

    for (const auto& agentPair : agents) {

        unsigned aux = agentPair.second.goalSite().GetStep();

        if (aux > maxStep) maxStep = aux;

    }

    return maxStep;

}

void _ga_token::error_edge_collision_check() const {

    unsigned count1 = agents.size();

    for (const auto& pagent1 : agents) {

        unsigned count2 = count1;

        for (const auto& pagent2 : agents) {

            if (count2 > 0) {

                if (pagent1.second.id() != pagent2.second.id() &&
                        pagent1.second.previousSite().match(pagent2.second.currentSite()) &&
                        pagent2.second.previousSite().match(pagent1.second.currentSite())) {

                    try {
                        std::ostringstream stream;
                        stream << "edge collision between agent " << pagent1.second.id();
                        stream << " and agent " << pagent2.second.id() << " step(" << currentStep << ")";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

                count2--;

            } else {

                break;

            }

        }

        count1--;

    }

}