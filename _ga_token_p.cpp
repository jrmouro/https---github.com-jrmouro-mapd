/* 
 * File:   _ga_token_p.cpp
 * Author: ronaldo
 * 
 * Created on 10 de novembro de 2023, 07:24
 */

#include "_ga_token_p.h"

_ga_token_p::_ga_token_p(
        const _map& map, 
        const _stepMap& stepMap, 
        const _agent_energy_system& agent_energy_system) :
            _ga_token(map, stepMap, agent_energy_system) { }

_ga_token_p::_ga_token_p(const _ga_token_p& other) : _ga_token(other) { }

_ga_token_p::~_ga_token_p() {}

_ga_token* _ga_token_p::getClone()const{
    return new _ga_token_p(*this);
}

std::string _ga_token_p::id() const {
    return "GAT_P";
}

bool _ga_token_p::updateAgentTaskPath_pendingTask(_ga_agent& agent, int newTaskId, bool trivialPath){
    
    bool ret = false;
    
    agent.unassignTask();
    agent.setStateFree();
    
    _stepPath newAgentPath(agent.currentSite());
        
    std::map<int, _task>::const_iterator new_task_it = pendingTasks.find(newTaskId);

    if (new_task_it != pendingTasks.end()) {

        const _task pendingTask = new_task_it->second;
        
        runningTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
        pendingTasks.erase(new_task_it);
        assignTaskAgent.insert(std::pair<int, int>(pendingTask.id(), agent.id()));
        
        agent.assignTask(pendingTask);
        agent.setStateFree();        
        ret = true;
        
        if (pendingTask.getPickup().match(pendingTask.getDelivery())) { // innocuous pending task   

            finishedTasks.insert(std::pair<int, _task>(pendingTask.id(), pendingTask));
            runningTasks.erase(new_task_it);
            agent.unassignTask();
                        
            if(!trivialPath){
                
                return true;
                
            }

        } else { // go to pickup   
            
            if(!trivialPath) stepMap.deleteMoving(agent.getPath(), agent.id());
            
            if (agent.currentSite().match(pendingTask.getPickup())) {
                
                agent.setStateBuzy();

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

bool _ga_token_p::updateAgentTaskPath_pickuping(_ga_agent& agent, int newTaskId) {

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
    
    std::map<int, _task>::const_iterator running_task_it = runningTasks.find(currentTask->id());

    if (running_task_it != runningTasks.end()) {
    
        if(currentTask->getPickup().match(currentTask->getDelivery())){

            finishedTasks.insert(std::pair<int, _task>(currentTask->id(), *currentTask));
            runningTasks.erase(currentTask->id());
            agent.setStateFree();
            agent.unassignTask();

            return updateAgentTaskPath_pendingTask(agent, newTaskId, true);

        } else {

            agent.setStateBuzy();

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
            stream << "running task not found: " << currentTask->id();
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    return false;

}

bool _ga_token_p::updateAgentTaskPath_going_to_rest_pickuping_to_pickuping(_ga_agent& agent, int newTaskId) {

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
    
    std::map<int, _task>::const_iterator running_task_it = runningTasks.find(currentTask->id());

    if (running_task_it != runningTasks.end()) {

        _stepAstarAlgorithm astar;

        _stepPath newAgentPath(agent.currentSite());

        agent.setStateBuzy();

        stepMap.deleteMoving(agent.getPath(), agent.id());

        liberateEndpoint(agent, currentTask->getDelivery());

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
            stream << "running task not found: " << currentTask->id();
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    return false;

}

bool _ga_token_p::updateAgentTaskPath(
        int agentId,
        int newTaskId) {
    
//        if(currentStep >= 200)
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

                } else  if (agent.isAtResting()) {
                    
//                    std::cout << "agent.isAtResting() : "<< std::endl;

                    return updateAgentTaskPath_resting(agent, newTaskId);

                } else if (agent.isAtRestPickuping()) {
                    
//                    std::cout << "agent.isAtRestPickuping() : "<< std::endl;
                    
                    try {
                        std::ostringstream stream;
                        stream << "unrecognized state";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
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

            } else if (agent.isBuzy()) {
                
                if (agent.isAtDelivering()) {
                    
//                    std::cout << "agent.isAtDelivering() : "<< std::endl;

                    return updateAgentTaskPath_delivering(agent, newTaskId);

                } else if (agent.isAtRestDelivering()) {
                    
//                    std::cout << "agent.isAtRestDelivering() : "<< std::endl;
                    
                    try {
                        std::ostringstream stream;
                        stream << "unrecognized state";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
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
                    
                    // nothing

                } else if (agent.isGoingToRestPickuping()) {
                    
//                    std::cout << "*agent.isGoingToRestPickuping() : "<< std::endl;
                    
                    // nothing
                    

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
                
                if (agent.isGoingToPickuping()) {
                    
//                    std::cout << "*agent.isGoingToPickuping() : "<< std::endl;
                    
                    try {
                        std::ostringstream stream;
                        stream << "unrecognized state";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                } else if (agent.isGoingToDelivering()) {
                    
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

