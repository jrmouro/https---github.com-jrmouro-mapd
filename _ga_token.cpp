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
    Writable::strWrite(*this, fs, name(), true);
    Writable::uintWrite(*this, fs, currentStep, true);
    Writable::strWrite(*this, fs, agent_energy_system.id(), true);
    Writable::intWrite(*this, fs, energyExpenditure(), true);
    Writable::uintWrite(*this, fs, finishedTasks.size(), false);
}

unsigned _ga_token::getCurrentStep() const {
    return currentStep;
}

void _ga_token::stepping() {

    //    if(currentStep == 53){
    //        
    //        for(int i = 0; i < 10; i++){
    //            
    //            stepMap.stepView(currentStep + i);
    //            
    //        }
    //        
    //        std::cout << std::endl;
    //        
    //    }
    
    if(currentStep > 1000)
        stepMap.stepView(currentStep);

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

std::string _ga_token::id() const {
    return "GAT";
}

std::string _ga_token::name() const {

    std::stringstream s;
    s << "GAT[" << agent_energy_system.id() << "]";

    return s.str();
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
    return pendingTasks.empty() &&
//            assignedTasks.empty() &&
            runningTasks.empty();
}

//bool _ga_token::liberateEndpoint(const _ga_agent& agent, const _site& endpoint) {
//
//    for (auto &agentPair : agents) {
//
//        if (agent.id() != agentPair.second.id() && agentPair.second.goalSite().match(endpoint)) { //other agents
//
//            return updateAgentRestPath(agentPair.second);
//
//        }
//
//    }
//
//    return true;
//
//}

_ga_agent* _ga_token::endpointObstruction(const _ga_agent& agent, const _site& endpoint) {

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

    listNonTaskEndpoints([closerEndpointIndexerAlgorithm, &vsite, agent, closerEndpoint, this](const _site & endpoint) {

        closerEndpointIndexerAlgorithm.ga_solve_to_site(*this, endpoint, closerEndpoint, vsite);

        return false;

    });

    for (auto endpoint : vsite) {

        bool flag = true;

        listConstAgents([endpoint, agent, &flag](unsigned index, const _ga_agent & otherAgent) {

            if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents

                flag = false;

                return true;

            }

            return false;

        });

        if (flag) {

            _stepPath newAgentPath(agent.getPath());
            bool flag = astar.solve(stepMap, newAgentPath, endpoint, agent.id());

            if (flag) {

                if (newAgentPath.isTrivial()) {

                    _stepSite future = agent.currentSite();
                    future.SetStep(future.GetStep() + 1);
                    newAgentPath.progress(future);

                }

                agent.assignPath(newAgentPath);
                stepMap.setMoving(newAgentPath, agent.id());

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

//bool _ga_token::updateAgentRestPath(_ga_agent& agent) {
//
//
//    if (agent.isGoingToPickuping()) {
//
//        return updateAgentPickupRestPath(agent);
//
//    }
//
//    _stepAstarAlgorithm astar;
//
//    _closerEndpointIndexerAlgorithm closerEndpointIndexerAlgorithm;
//
//    std::vector<_site> vsite;
//
//    listNonTaskEndpoints([closerEndpointIndexerAlgorithm, &vsite, agent, this](const _site & endpoint) {
//
//        closerEndpointIndexerAlgorithm.ga_solve_to_agent(*this, endpoint, agent, vsite);
//
//        return false;
//
//    });
//
//    for (auto endpoint : vsite) {
//
//        bool flag = true;
//
//        listConstAgents([endpoint, agent, &flag](unsigned index, const _ga_agent & otherAgent) {
//
//            if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents
//
//                flag = false;
//
//                return true;
//
//            }
//
//            return false;
//
//        });
//
//        if (flag) {
//
//            const _task* currentTask = agent.getCurrentTask();
//
//            if (agent.isGoingToDelivering()) {
//
//                if (currentTask != nullptr) {
//
//                    std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(currentTask->id());
//
//                    if (currentTask_it != runningTasks.end()) {
//
//                        agent.unassignTask();
//
//                        finishedTasks.insert(std::pair<int, _task>(currentTask_it->first, currentTask_it->second));
//                        runningTasks.erase(currentTask_it);
//
//                    } else {
//
//                        try {
//                            std::ostringstream stream;
//                            stream << "running task id not found: " << currentTask->id();
//                            MAPD_EXCEPTION(stream.str());
//                        } catch (std::exception& e) {
//                            std::cout << e.what() << std::endl;
//                            std::abort();
//                        }
//
//                    }
//
//                } else {
//
//                    try {
//                        std::ostringstream stream;
//                        stream << "nill current task";
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }
//
//                }
//
//            }
//
//            _stepPath newAgentPath(agent.goalSite());
//            bool flag = astar.solve(stepMap, newAgentPath, endpoint, agent.id());
//
//            if (flag) {
//
//                agent.assignPath(newAgentPath);
//                stepMap.setMoving(newAgentPath, agent.id());
//
//                return true;
//
//            } else {
//
//                try {
//                    std::ostringstream stream;
//                    stream << "no task endpoint site path not found" << endpoint;
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                }
//
//            }
//
//        } else {
//
//            try {
//                std::ostringstream stream;
//                stream << "no endpoint available";
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//        }
//
//    }
//
//    return false;
//
//}
//
//bool _ga_token::updateAgentPickupRestPath(_ga_agent& agent) {
//
//    if (agent.isGoingToPickuping()) {
//
//        const _task* currentTask = agent.getCurrentTask();
//
//        _closerEndpointIndexerAlgorithm closerEndpointIndexerAlgorithm;
//
//        std::vector<_site> vsite;
//
//        listNonTaskEndpoints([closerEndpointIndexerAlgorithm, &vsite, currentTask, this](const _site & endpoint) {
//
//            closerEndpointIndexerAlgorithm.ga_solve_to_site(*this, endpoint, currentTask->getPickup(), vsite);
//
//            return false;
//
//        });
//
//        for (auto endpoint : vsite) {
//
//            bool flag = true;
//
//            listConstAgents([endpoint, agent, &flag](unsigned index, const _ga_agent & otherAgent) {
//
//                if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents
//
//                    flag = false;
//
//                    return true;
//
//                }
//
//                return false;
//
//            });
//
//            if (flag) {
//
//                _stepAstarAlgorithm astar;
//
//                _stepPath newAgentPath(agent.currentSite());
//                bool flag = astar.solve(stepMap, newAgentPath, endpoint, agent.id());
//
//                if (flag) {
//
////                    std::map<int, _task>::const_iterator currentTaskIt = assignedTasks.find(currentTask->id());
////
////                    if (currentTaskIt != assignedTasks.end()) {
//
//                        runningTasks.insert(std::pair<int, _task>(currentTaskIt->first, currentTaskIt->second));
////                        assignedTasks.erase(currentTaskIt);
//
////                    } else {
////
////                        try {
////                            std::ostringstream stream;
////                            stream << "assigned task id not found: " << currentTask->id();
////                            MAPD_EXCEPTION(stream.str());
////                        } catch (std::exception& e) {
////                            std::cout << e.what() << std::endl;
////                            std::abort();
////                        }
////
////                    }
//
//                    if (newAgentPath.isTrivial()) {
//
//                        _stepSite future = agent.currentSite();
//                        future.SetStep(future.GetStep() + 1);
//                        newAgentPath.progress(future);
//
//                    }
//
//                    agent.assignPath(newAgentPath);
//                    stepMap.setMoving(newAgentPath, agent.id());
//
//                    return true;
//
//                } else {
//
//                    try {
//                        std::ostringstream stream;
//                        stream << "no task endpoint site path not found" << endpoint;
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }
//
//                }
//
//            } else {
//
//                try {
//                    std::ostringstream stream;
//                    stream << "no endpoint available";
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                }
//
//            }
//
//        }
//
//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "agent is not pickuping";
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }
//
//    return false;
//
//}

bool _ga_token::updateAgentTaskPath_resting(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    _stepPath newAgentPath(agent.currentSite());

    std::map<int, _task>::const_iterator new_task_it = pendingTasks.find(newTaskId);

    if (new_task_it != pendingTasks.end()) {

        const _task& pendingTask = new_task_it->second;

        if (pendingTask.getPickup().match(pendingTask.getDelivery())) { // innocuous pending task   

            finishedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
            pendingTasks.erase(new_task_it);
            assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));

            ret = true;

        } else { // go to pickup      

            if (agent.currentSite().match(pendingTask.getPickup())) {

                runningTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
                pendingTasks.erase(new_task_it);
                assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));

                agent.assignTask(pendingTask);
                agent.setStateBuzy();
                
                ret = true;

                bool flag = endpointObstruction(agent, pendingTask.getDelivery()) == nullptr;

                if (flag) {

                    _stepAstarAlgorithm astar;

                    flag = astar.solve(stepMap, newAgentPath, pendingTask.getDelivery(), agent.id());

                    if (!flag) {

                        try {
                            std::ostringstream stream;
                            stream << "delivery site path not found: " << pendingTask.getDelivery();
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }

                    }

                } else {

                    return updateAgentRestPathCloserEndpoint(agent, pendingTask.getDelivery());

                }

            } else {
                
//                assignedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));

                agent.assignTask(pendingTask);
                agent.setStateFree();

                bool flag = endpointObstruction(agent, pendingTask.getPickup()) == nullptr;

                if (flag) {

                    _stepAstarAlgorithm astar;

                    flag = astar.solve(stepMap, newAgentPath, pendingTask.getPickup(), agent.id());

                    if (!flag) {

                        try {
                            std::ostringstream stream;
                            stream << "pickup site path not found: " << pendingTask.getDelivery();
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }

                    }

                }

            }

        }

    }

    if (newAgentPath.isTrivial()) {

        bool flag = endpointObstruction(agent, agent.currentSite()) != nullptr;

        if (flag) {

            try {
                std::ostringstream stream;
                stream << "no task endpoint error: " << agent.currentSite();
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

        _stepSite future = agent.currentSite();
        future.SetStep(future.GetStep() + 1);
        newAgentPath.progress(future);

    }

    agent.assignPath(newAgentPath);
    stepMap.setMoving(newAgentPath, agent.id());

    return ret;

}

bool _ga_token::updateAgentTaskPath_going_to_resting(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    

    std::map<int, _task>::const_iterator new_task_it = pendingTasks.find(newTaskId);

    if (new_task_it != pendingTasks.end()) {
        
        

        const _task& pendingTask = new_task_it->second;

        if (pendingTask.getPickup().match(pendingTask.getDelivery())) { // innocuous pending task   

            finishedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
            pendingTasks.erase(new_task_it);
            assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));

            return true;

        } else { // go to pickup   
            
            _stepPath newAgentPath(agent.currentSite());
            
//            assignedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));

            agent.assignTask(pendingTask);
            agent.setStateFree();

            stepMap.deleteMoving(agent.getPath(), agent.id());

            bool flag = endpointObstruction(agent, pendingTask.getPickup()) == nullptr;

            if (flag) {
                
                _stepAstarAlgorithm astar;

                flag = astar.solve(stepMap, newAgentPath, pendingTask.getPickup(), agent.id());
                
                if(!flag){

                    try {
                        std::ostringstream stream;
                        stream << "pickup site path not found: " << pendingTask.getPickup();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            } else {
                
                return !updateAgentRestPathCloserEndpoint(agent, pendingTask.getPickup());

            }
            
            if (newAgentPath.isTrivial()) {

                _stepSite future = agent.currentSite();
                future.SetStep(future.GetStep() + 1);
                newAgentPath.progress(future);

            }

            agent.assignPath(newAgentPath);
            stepMap.setMoving(newAgentPath, agent.id());

        }

    }
    
    return false;

}

bool _ga_token::updateAgentTaskPath_pickuping(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    _stepPath newAgentPath(agent.currentSite());

    const _task* currentTask = agent.getCurrentTask();

//    std::map<int, _task>::const_iterator currentTask_it = assignedTasks.find(currentTask->id());
//
//    if (currentTask_it != assignedTasks.end()) {

        if (currentTask->id() == newTaskId) {

//            assignedTasks.erase(currentTask->id());
            pendingTasks.erase(currentTask->id());
            runningTasks.insert(std::pair<int, _task>(currentTask->id(), *currentTask));
            assignTaskAgent.insert(std::pair<int, int>(currentTask->id(), agent.id()));

            agent.setStateBuzy();
//            agent.assignTask(*currentTask);
            
            ret = true;

            bool flag = endpointObstruction(agent, currentTask->getDelivery()) == nullptr;

            if (flag) {

                _stepAstarAlgorithm astar;

                flag = astar.solve(stepMap, newAgentPath, currentTask->getDelivery(), agent.id());

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

            } else {

                return updateAgentRestPathCloserEndpoint(agent, currentTask->getDelivery());

            }

        } else { // different tasks

            std::map<int, _task>::const_iterator new_task_it = pendingTasks.find(newTaskId);

            if (new_task_it != pendingTasks.end()) {

                const _task& pendingTask = new_task_it->second;

                if (pendingTask.getPickup().match(pendingTask.getDelivery())) { // innocuous pending task   

                    finishedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
                    pendingTasks.erase(new_task_it);
                    assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));

                    agent.unassignTask();
                    agent.setStateFree();

                    return updateAgentRestPathCloserEndpoint(agent, agent.currentSite());
                    ;

                } else { // go to pickup    

//                    assignedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));

                    agent.assignTask(pendingTask);
                    agent.setStateFree();

                    bool flag = endpointObstruction(agent, pendingTask.getPickup()) == nullptr;

                    if (flag) {

                        _stepAstarAlgorithm astar;

                        flag = astar.solve(stepMap, newAgentPath, pendingTask.getPickup(), agent.id());

                        if (!flag) {

                            try {
                                std::ostringstream stream;
                                stream << "pickup site path not found: " << pendingTask.getDelivery();
                                MAPD_EXCEPTION(stream.str());
                            } catch (std::exception& e) {
                                std::cout << e.what() << std::endl;
                                std::abort();
                            }

                        }

                    } else {

                        return !updateAgentRestPathCloserEndpoint(agent, pendingTask.getPickup());

                    }

                }

            }

        }

        if (newAgentPath.isTrivial()) {

            _stepSite future = agent.currentSite();
            future.SetStep(future.GetStep() + 1);
            newAgentPath.progress(future);

        }

        agent.assignPath(newAgentPath);
        stepMap.setMoving(newAgentPath, agent.id());

//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "assigned task id not found: " << currentTask->id();
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }

    return ret;

}

bool _ga_token::updateAgentTaskPath_rest_pickuping(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    _stepPath newAgentPath(agent.currentSite());

    const _task* currentTask = agent.getCurrentTask();

//    std::map<int, _task>::const_iterator currentTask_it = assignedTasks.find(currentTask->id());

//    if (currentTask_it != assignedTasks.end()) {

        if (currentTask->id() == newTaskId) {

            bool flag = endpointObstruction(agent, currentTask->getPickup()) == nullptr;

            if (flag) {

                _stepAstarAlgorithm astar;

                flag = astar.solve(stepMap, newAgentPath, currentTask->getPickup(), agent.id());

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

            }

        } else { // different tasks
            
//            assignedTasks.erase(currentTask->id());
            agent.unassignTask();
            agent.setStateFree();

            std::map<int, _task>::const_iterator new_task_it = pendingTasks.find(newTaskId);

            if (new_task_it != pendingTasks.end()) {

                const _task& pendingTask = new_task_it->second;

                if (pendingTask.getPickup().match(pendingTask.getDelivery())) { // innocuous pending task   

                    finishedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
                    pendingTasks.erase(new_task_it);
                    assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));

                    agent.unassignTask();
                    agent.setStateFree();

                    ret = true;

                } else { // go to pickup    
                                        
//                    assignedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));

                    agent.assignTask(pendingTask);
                    agent.setStateFree();

                    bool flag = endpointObstruction(agent, pendingTask.getPickup()) == nullptr;

                    if (flag) {

                        _stepAstarAlgorithm astar;

                        flag = astar.solve(stepMap, newAgentPath, pendingTask.getPickup(), agent.id());

                        if (!flag) {

                            try {
                                std::ostringstream stream;
                                stream << "pickup site path not found: " << pendingTask.getDelivery();
                                MAPD_EXCEPTION(stream.str());
                            } catch (std::exception& e) {
                                std::cout << e.what() << std::endl;
                                std::abort();
                            }

                        }

                    } else {

                        return !updateAgentRestPathCloserEndpoint(agent, pendingTask.getPickup());

                    }

                }

            }

        }

        if (newAgentPath.isTrivial()) {

            _stepSite future = agent.currentSite();
            future.SetStep(future.GetStep() + 1);
            newAgentPath.progress(future);

        }

        agent.assignPath(newAgentPath);
        stepMap.setMoving(newAgentPath, agent.id());

//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "assigned task id not found: " << currentTask->id();
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }

    return ret;

}

bool _ga_token::updateAgentTaskPath_going_to_pickuping(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    const _task* currentTask = agent.getCurrentTask();

//    std::map<int, _task>::const_iterator currentTask_it = assignedTasks.find(currentTask->id());
//
//    if (currentTask_it != assignedTasks.end()) {

        if (newTaskId == currentTask->id()) {

            return false;

        } else {

//            assignedTasks.erase(currentTask_it);
            agent.unassignTask();

            std::map<int, _task>::const_iterator newTask_it = pendingTasks.find(newTaskId);

            if (newTask_it != pendingTasks.end()) {

                const _task& newTask = newTask_it->second;

                if (newTask.getPickup().match(newTask.getDelivery())) { // innocuous newTask

                    pendingTasks.erase(newTask_it);
                    finishedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                    assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                    agent.setStateFree();
                    agent.unassignTask();

                    return updateAgentRestPathCloserEndpoint(agent, agent.currentSite());

                } else {

                    _stepAstarAlgorithm astar;

                    _stepPath newAgentPath(agent.currentSite());

                    stepMap.deleteMoving(agent.getPath(), agent.id());

                    if (agent.currentSite().match(newTask.getPickup())) {

                        runningTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                        pendingTasks.erase(newTask_it);
                        assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                        agent.assignTask(newTask);
                        agent.setStateBuzy();

                        ret = true;

                        bool flag = endpointObstruction(agent, newTask.getDelivery()) == nullptr;

                        if (flag) {

                            flag = astar.solve(stepMap, newAgentPath, newTask.getDelivery(), agent.id());

                            if (!flag) {

                                try {
                                    std::ostringstream stream;
                                    stream << "delivery site path not found: " << newTask.getDelivery();
                                    MAPD_EXCEPTION(stream.str());
                                } catch (std::exception& e) {
                                    std::cout << e.what() << std::endl;
                                    std::abort();
                                }

                            }

                        } else {

                            agent.assignPath(newAgentPath);
                            stepMap.setMoving(agent.getPath(), agent.id());

                            return updateAgentRestPathCloserEndpoint(agent, newTask.getDelivery());

                        }

                    } else {

//                        assignedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));

                        agent.assignTask(newTask);
                        agent.setStateFree();

                        ret = false;

                        bool flag = endpointObstruction(agent, newTask.getPickup()) == nullptr;

                        if (flag) {

                            flag = astar.solve(stepMap, newAgentPath, newTask.getPickup(), agent.id());

                            if (!flag) {

                                try {
                                    std::ostringstream stream;
                                    stream << "pickup site path not found: " << newTask.getPickup();
                                    MAPD_EXCEPTION(stream.str());
                                } catch (std::exception& e) {
                                    std::cout << e.what() << std::endl;
                                    std::abort();
                                }

                            }

                        } else {

                            agent.assignPath(newAgentPath);
                            stepMap.setMoving(agent.getPath(), agent.id());

                            return !updateAgentRestPathCloserEndpoint(agent, newTask.getPickup());

                        }

                    }

                    if (newAgentPath.isTrivial()) {

                        _stepSite future = agent.currentSite();
                        future.SetStep(future.GetStep() + 1);
                        newAgentPath.progress(future);

                    }

                    agent.assignPath(newAgentPath);
                    stepMap.setMoving(newAgentPath, agent.id());

                }

            } 

        }

//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "assigned task id not found: " << currentTask->id();
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }

    return ret;

}

bool _ga_token::updateAgentTaskPath_going_to_rest_pickuping(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    const _task* currentTask = agent.getCurrentTask();

//    std::map<int, _task>::const_iterator currentTask_it = assignedTasks.find(currentTask->id());
//
//    if (currentTask_it != assignedTasks.end()) {

        if (newTaskId == currentTask->id()) {

            return false;

        } else {

//            assignedTasks.erase(currentTask_it);
            agent.unassignTask();

            std::map<int, _task>::const_iterator newTask_it = pendingTasks.find(newTaskId);

            if (newTask_it != pendingTasks.end()) {

                const _task& newTask = newTask_it->second;

                if (newTask.getPickup().match(newTask.getDelivery())) { // innocuous newTask

                    pendingTasks.erase(newTask_it);
                    finishedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                    assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                    agent.setStateFree();
                    agent.unassignTask();

                    return updateAgentRestPathCloserEndpoint(agent, agent.currentSite());

                } else {

                    _stepAstarAlgorithm astar;

                    _stepPath newAgentPath(agent.currentSite());

                    stepMap.deleteMoving(agent.getPath(), agent.id());

                    if (agent.currentSite().match(newTask.getPickup())) {

                        runningTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                        pendingTasks.erase(newTask_it);
                        assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                        agent.assignTask(newTask);
                        agent.setStateBuzy();

                        ret = true;

                        bool flag = endpointObstruction(agent, newTask.getDelivery()) == nullptr;

                        if (flag) {

                            flag = astar.solve(stepMap, newAgentPath, newTask.getDelivery(), agent.id());

                            if (!flag) {

                                try {
                                    std::ostringstream stream;
                                    stream << "delivery site path not found: " << newTask.getDelivery();
                                    MAPD_EXCEPTION(stream.str());
                                } catch (std::exception& e) {
                                    std::cout << e.what() << std::endl;
                                    std::abort();
                                }

                            }

                        } else {

                            agent.assignPath(newAgentPath);
                            stepMap.setMoving(agent.getPath(), agent.id());

                            return updateAgentRestPathCloserEndpoint(agent, newTask.getDelivery());

                        }

                    } else {

//                        assignedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));

                        agent.assignTask(newTask);
                        agent.setStateFree();

                        ret = false;

                        bool flag = endpointObstruction(agent, newTask.getPickup()) == nullptr;

                        if (flag) {

                            flag = astar.solve(stepMap, newAgentPath, newTask.getPickup(), agent.id());

                            if (!flag) {

                                try {
                                    std::ostringstream stream;
                                    stream << "pickup site path not found: " << newTask.getPickup();
                                    MAPD_EXCEPTION(stream.str());
                                } catch (std::exception& e) {
                                    std::cout << e.what() << std::endl;
                                    std::abort();
                                }

                            }

                        } else {

                            agent.assignPath(newAgentPath);
                            stepMap.setMoving(agent.getPath(), agent.id());

                            return !updateAgentRestPathCloserEndpoint(agent, newTask.getPickup());

                        }

                    }

                    if (newAgentPath.isTrivial()) {

                        _stepSite future = agent.currentSite();
                        future.SetStep(future.GetStep() + 1);
                        newAgentPath.progress(future);

                    }

                    agent.assignPath(newAgentPath);
                    stepMap.setMoving(newAgentPath, agent.id());

                }

            } 

        }

//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "assigned task id not found: " << currentTask->id();
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }

    return ret;

}

bool _ga_token::updateAgentTaskPath_going_to_rest_pickuping_to_pickuping(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    const _task* currentTask = agent.getCurrentTask();

//    std::map<int, _task>::const_iterator currentTask_it = assignedTasks.find(currentTask->id());
//
//    if (currentTask_it != assignedTasks.end()) {
        
        _stepAstarAlgorithm astar;

        _stepPath newAgentPath(agent.currentSite());
        
//        assignedTasks.erase(currentTask_it);

        if (newTaskId == currentTask->id()) {
            
            runningTasks.insert(std::pair<int, _task>(currentTask->id(), *currentTask));
            pendingTasks.erase(currentTask->id());
            assignTaskAgent.insert(std::pair<int, int>(currentTask->id(), agent.id()));
            
            agent.setStateBuzy();
            
            stepMap.deleteMoving(agent.getPath(), agent.id());
            
            ret = true;
            
            bool flag = endpointObstruction(agent, currentTask->getDelivery()) == nullptr;

            if (flag) {

                flag = astar.solve(stepMap, newAgentPath, currentTask->getDelivery(), agent.id());

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

            } else {

                agent.assignPath(newAgentPath);
                stepMap.setMoving(agent.getPath(), agent.id());

                return updateAgentRestPathCloserEndpoint(agent, currentTask->getDelivery());

            }            
            
        } else {
            
            agent.setStateFree();
            agent.unassignTask();
            
            std::map<int, _task>::const_iterator newTask_it = pendingTasks.find(newTaskId);

            if (newTask_it != pendingTasks.end()) {

                const _task& newTask = newTask_it->second;

                if (newTask.getPickup().match(newTask.getDelivery())) { // innocuous newTask

                    pendingTasks.erase(newTask_it);
                    finishedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                    assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                    agent.setStateFree();
                    agent.unassignTask();

                    return true;

                } else {

                    stepMap.deleteMoving(agent.getPath(), agent.id());

                    if (agent.currentSite().match(newTask.getPickup())) {

                        runningTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                        pendingTasks.erase(newTask_it);
                        assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                        agent.assignTask(newTask);
                        agent.setStateBuzy();

                        ret = true;

                        bool flag = endpointObstruction(agent, newTask.getDelivery()) == nullptr;

                        if (flag) {

                            flag = astar.solve(stepMap, newAgentPath, newTask.getDelivery(), agent.id());

                            if (!flag) {

                                try {
                                    std::ostringstream stream;
                                    stream << "delivery site path not found: " << newTask.getDelivery();
                                    MAPD_EXCEPTION(stream.str());
                                } catch (std::exception& e) {
                                    std::cout << e.what() << std::endl;
                                    std::abort();
                                }

                            }

                        } else {

                            agent.assignPath(newAgentPath);
                            stepMap.setMoving(agent.getPath(), agent.id());

                            return updateAgentRestPathCloserEndpoint(agent, newTask.getDelivery());

                        }

                    } else {

//                        assignedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));

                        agent.assignTask(newTask);
                        agent.setStateFree();

                        ret = false;

                        bool flag = endpointObstruction(agent, newTask.getPickup()) == nullptr;

                        if (flag) {

                            flag = astar.solve(stepMap, newAgentPath, newTask.getPickup(), agent.id());

                            if (!flag) {

                                try {
                                    std::ostringstream stream;
                                    stream << "pickup site path not found: " << newTask.getPickup();
                                    MAPD_EXCEPTION(stream.str());
                                } catch (std::exception& e) {
                                    std::cout << e.what() << std::endl;
                                    std::abort();
                                }

                            }

                        } else {

                            agent.assignPath(newAgentPath);
                            stepMap.setMoving(agent.getPath(), agent.id());

                            return !updateAgentRestPathCloserEndpoint(agent, newTask.getPickup());

                        }

                    }                    

                }

            } 

        }
        
        if (newAgentPath.isTrivial()) {

            _stepSite future = agent.currentSite();
            future.SetStep(future.GetStep() + 1);
            newAgentPath.progress(future);

        }

        agent.assignPath(newAgentPath);
        stepMap.setMoving(newAgentPath, agent.id());

//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "assigned task id not found: " << currentTask->id();
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }

    return ret;

}

bool _ga_token::updateAgentTaskPath_delivering(_ga_agent& agent, int newTaskId) {

    bool ret = false;

    _stepPath newAgentPath(agent.currentSite());

    const _task* currentTask = agent.getCurrentTask();

    std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(currentTask->id());

    if (currentTask_it != runningTasks.end()) {

        finishedTasks.insert(std::pair<int, _task>(currentTask_it->first, currentTask_it->second));
        runningTasks.erase(currentTask_it);

        agent.unassignTask();
        agent.setStateFree();

        std::map<int, _task>::const_iterator new_task_it = pendingTasks.find(newTaskId);

        if (new_task_it != pendingTasks.end()) {

            const _task& pendingTask = new_task_it->second;

            if (pendingTask.getPickup().match(agent.currentSite())) { // pickup

                if (pendingTask.getDelivery().match(agent.currentSite())) { // innocuous task

                    finishedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
                    pendingTasks.erase(new_task_it);
                    assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));

                    agent.unassignTask();
                    agent.setStateFree();

                    return updateAgentRestPathCloserEndpoint(agent, agent.currentSite());

                } else { // go to delivery

                    runningTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
                    pendingTasks.erase(new_task_it);
                    assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));

                    agent.assignTask(pendingTask);
                    agent.setStateBuzy();

                    ret = true;

                    bool flag = endpointObstruction(agent, pendingTask.getDelivery()) == nullptr;

                    if (flag) {

                        _stepAstarAlgorithm astar;

                        flag = astar.solve(stepMap, newAgentPath, pendingTask.getDelivery(), agent.id());

                        if (!flag) {

                            try {
                                std::ostringstream stream;
                                stream << "delivery site path not found: " << pendingTask.getDelivery();
                                MAPD_EXCEPTION(stream.str());
                            } catch (std::exception& e) {
                                std::cout << e.what() << std::endl;
                                std::abort();
                            }

                        }

                    } else {

                        return updateAgentRestPathCloserEndpoint(agent, pendingTask.getDelivery());

                    }

                }

            } else { // go to pickup

//                assignedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));

                agent.assignTask(pendingTask);
                agent.setStateFree();

                bool flag = endpointObstruction(agent, pendingTask.getPickup()) == nullptr;

                if (flag) {

                    _stepAstarAlgorithm astar;

                    flag = astar.solve(stepMap, newAgentPath, pendingTask.getPickup(), agent.id());

                    if (!flag) {

                        try {
                            std::ostringstream stream;
                            stream << "pickup site path not found: " << pendingTask.getPickup();
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }

                    }

                } else {

                    return !updateAgentRestPathCloserEndpoint(agent, pendingTask.getPickup());

                }

            }

        } else {
            
            return !updateAgentRestPathCloserEndpoint(agent, agent.currentSite());
            
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

    return ret;

}

bool _ga_token::updateAgentTaskPath_rest_delivering(_ga_agent& agent) {

    _stepPath newAgentPath(agent.currentSite());
    const _task* currentTask = agent.getCurrentTask();

    std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(currentTask->id());

    if (currentTask_it != runningTasks.end()) {

        bool flag = endpointObstruction(agent, currentTask->getDelivery()) == nullptr;

        if (flag) {

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

    bool ret = false;

    const _task* currentTask = agent.getCurrentTask();

    std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(currentTask->id());

    if (currentTask_it != runningTasks.end()) {
        
        _stepAstarAlgorithm astar;

        _stepPath newAgentPath(agent.currentSite());
        
        runningTasks.erase(currentTask_it);
        finishedTasks.insert(std::pair<int, _task>(currentTask->id(), *currentTask));
        assignTaskAgent.insert(std::pair<int, int>(currentTask->id(), agent.id()));

        agent.setStateFree();
        agent.unassignTask();
        
        std::map<int, _task>::const_iterator newTask_it = pendingTasks.find(newTaskId);

        if (newTask_it != pendingTasks.end()) {

            const _task& newTask = newTask_it->second;

            if (newTask.getPickup().match(newTask.getDelivery())) { // innocuous newTask

                pendingTasks.erase(newTask_it);
                finishedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                agent.setStateFree();
                agent.unassignTask();
                
                return true;
                
            } else {

                stepMap.deleteMoving(agent.getPath(), agent.id());

                if (agent.currentSite().match(newTask.getPickup())) {

                    runningTasks.insert(std::pair<int, _task>(newTask.id(), newTask));
                    pendingTasks.erase(newTask_it);
                    assignTaskAgent.insert(std::pair<int, int>(newTask.id(), agent.id()));

                    agent.assignTask(newTask);
                    agent.setStateBuzy();

                    ret = true;

                    bool flag = endpointObstruction(agent, newTask.getDelivery()) == nullptr;

                    if (flag) {

                        flag = astar.solve(stepMap, newAgentPath, newTask.getDelivery(), agent.id());

                        if (!flag) {

                            try {
                                std::ostringstream stream;
                                stream << "delivery site path not found: " << newTask.getDelivery();
                                MAPD_EXCEPTION(stream.str());
                            } catch (std::exception& e) {
                                std::cout << e.what() << std::endl;
                                std::abort();
                            }

                        }

                    } else {

                        agent.assignPath(newAgentPath);
                        stepMap.setMoving(agent.getPath(), agent.id());

                        return updateAgentRestPathCloserEndpoint(agent, newTask.getDelivery());

                    }

                } else {

//                    assignedTasks.insert(std::pair<int, _task>(newTask.id(), newTask));

                    agent.assignTask(newTask);
                    agent.setStateFree();

                    ret = false;

                    bool flag = endpointObstruction(agent, newTask.getPickup()) == nullptr;

                    if (flag) {

                        flag = astar.solve(stepMap, newAgentPath, newTask.getPickup(), agent.id());

                        if (!flag) {

                            try {
                                std::ostringstream stream;
                                stream << "pickup site path not found: " << newTask.getPickup();
                                MAPD_EXCEPTION(stream.str());
                            } catch (std::exception& e) {
                                std::cout << e.what() << std::endl;
                                std::abort();
                            }

                        }

                    } else {

                        agent.assignPath(newAgentPath);
                        stepMap.setMoving(agent.getPath(), agent.id());

                        return !updateAgentRestPathCloserEndpoint(agent, newTask.getPickup());

                    }

                }                    

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

    return ret;

}

bool _ga_token::updateAgentTaskPath(
        int agentId,
        int newTaskId) {

    std::map<int, _ga_agent>::iterator agent_it = agents.find(agentId);

    if (agent_it != agents.end()) {

        _ga_agent& agent = agent_it->second;

        if (agent.isAtTrivialPath()) {

            if (agent.isFree()) {

                if (agent.isAtPickuping()) {

                    return updateAgentTaskPath_pickuping(agent, newTaskId);

                } else if (agent.isAtResting()) {

                    return updateAgentTaskPath_resting(agent, newTaskId);

                } else if (agent.isAtRestPickuping()) {

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

                    return updateAgentTaskPath_delivering(agent, newTaskId);

                } else if (agent.isAtRestDelivering()) {

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

            if (agent.isFree()) {

                if (agent.isAtPickuping()) {

                    return updateAgentTaskPath_going_to_rest_pickuping_to_pickuping(agent, newTaskId);

                } else if (agent.isGoingToPickuping()) {

                    return updateAgentTaskPath_going_to_pickuping(agent, newTaskId);

                } else if (agent.isGoingToRestPickuping()) {

                    return updateAgentTaskPath_going_to_rest_pickuping(agent, newTaskId);

                } else if (agent.isGoingToResting()) {

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

                    // nothing

                } else if (agent.isAtDelivering()) {

                    return updateAgentTaskPath_going_to_rest_delivering_to_delivering(agent, newTaskId);

                } else if (agent.isGoingToRestDelivering()) {

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