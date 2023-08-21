/* 
 * File:   _agent.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 09:30
 */

#include "_agent.h"
#include "_agent_state.h"
#include "_token.h"
#include "_agent_free.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent::_agent(int _id, const _stepSite& currentSite) : 
_id(_id), _currentPath(currentSite), _state(_agent_free::getInstance()) { }

_agent::_agent(const _agent& other)  :
    _id(other._id), 
    _state(other._state), 
    _currentPath(other._currentPath), 
    tasks(other.tasks), 
    _currentTaskIndex(other._currentTaskIndex) { }


_agent::~_agent(){ }

void _agent::receive(_system& system) {
 
    this->_state->onUpdatePath(system, this);
        
}

void _agent::move(_system& system) {
    
    this->stepping();
        
    this->_state->onMoveUpdate(system, this);
    
}

bool _agent::updateTaskPath(_system& system){
    
    _task newTask;
    _stepPath path(this->currentSite());
    bool flag = system.getToken().selectNewTaskPathToAgent(*this, newTask, path);
    
    if(flag){
        
        system.getToken().assignTask(newTask, *this);
        system.getToken().reportTaskUpdate(newTask.id(), this->id(), ReportTask::PathType::task, path);
        
        this->designTask(newTask);
        this->setPathMoving(path, system.getToken().getStepMap());
        
        if (this->isParked()) {
                
            system.getToken().runTask((newTask));
            system.getToken().finishTask(newTask);

            this->undesignTask();                    

            this->setTrivialPathMoving(system.getStepMap());   

        } 

        return true;
        
    }
    
    
    return false;
    
//    _task newTask;
//        
//    if (this->selectNewTask(system, newTask)) {
//        
//        system.getToken().assignTask(newTask, *this);
//
//        this->designTask(newTask);
//
//        _stepPath path(this->currentSite());
//
//        if(this->taskPath(system.getStepMap(), newTask, path)){
//
//            system.getToken().reportTaskUpdate(newTask.id(), this->id(), ReportTask::PathType::task, path);  
//
//            this->setPathMoving(path, system.getStepMap());                                    
//
//            if (this->isParked()) {
//                
//                system.getToken().finishTask(newTask);
//                
//                this->undesignTask();                    
//
//                this->setTrivialPathMoving(system.getStepMap());   
//
//            } 
//            
//            return true;
//
//        } else {
//
//            try {
//                std::ostringstream stream;
//                stream << "unsolved task path: ";
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
    
}

bool _agent::updateRestPath(_system& system){
    
    _task conflict;
    
    _stepPath restPath(this->currentSite());
    
    if(system.getToken().selectNewRestEndpointPathToAgent(*this, conflict, restPath)){
        
        system.getToken().reportTaskUpdate(conflict.id(), this->id(), ReportTask::PathType::rest, restPath);  

        this->setPathMoving(restPath, system.getStepMap());
        
        return true;
        
    }
    
    return false;
    
    
//    _task conflit;
//
//    if (this->isConflictingRestEndpoint(system.getToken(), conflit)) {
//
//        _site endpoint;
//
//        if (this->selectNewRestEndpoint(system, endpoint)) {
//
//            _stepPath restPath(this->currentSite());
//
//            if(this->restEndpointPath(system.getStepMap(), endpoint, restPath)){
//
//                system.getToken().reportTaskUpdate(conflit.id(), this->id(), ReportTask::PathType::rest, restPath);  
//
//                this->setPathMoving(restPath, system.getStepMap());
//
//                return true;
//
//            }else{
//
//                try {
//                    std::ostringstream stream;
//                    stream << "unsolved rest endpoint path";
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
//                 stream << "new rest endpoint not found";
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//
//        }
//
//    }
//    
//    return false;
    
}

void _agent::updatePath(_system& system){
    
    if (this->isParked()) { // request token condition
        
        if (!this->isResting()){ //retirar depois
            
            try {
                std::ostringstream stream;
                stream << "no resting agent: " << *this;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }                                  
            
        } 
        
        if(!this->updateTaskPath(system))
            if(!this->updateRestPath(system))
                this->setTrivialPathMoving(system.getStepMap());
                     
    }

    
}


bool _agent::selectNewRestEndpoint(_system& system, _site& selectNewSite) {

    unsigned min_distance = 0xffffffff;

    system.listEndpoints([&system, &min_distance, &selectNewSite, this](const _site &endpoint) {
        
        bool flag = true;

        system.getToken().listPendingTasks([endpoint, &flag](const _task & task) {

            if (task.getDelivery().match(endpoint)) {

                flag = false;
                
                return true;                

            }

            return false;

        });
        
        if(flag){
        
            system.getToken().listAgents([endpoint, this, &flag](_agent & agent) {

                if (this->id() != agent.id() && agent.goalSite().match(endpoint)) { //other agents

                    flag = false;
                    
                    return true;

                } 

                return false;

            });
                
        }
        
        if(flag){
            
            unsigned distance = system.getEndpointsDistanceAlgorithm().solve(this->currentSite(), endpoint);
            
            if (distance < min_distance) {
                min_distance = distance;
                selectNewSite = endpoint;
            }
            
            
        }

        return false;

    });

    return min_distance < 0xffffffff;

}

bool _agent::selectNewTask(_system& system, _task& selectedTask) const{

    unsigned min_distance = 0xffffffff;
        
    system.getToken().listPendingTasks([&system, &min_distance, &selectedTask, this](const _task & task) {
        
        bool flag = true;
        
        system.getToken().listAgents([task, &flag, this](const _agent & agent) {
            
            if (this->id() != agent.id()) {
                
                if (agent.goalSite().match(task.getPickup()) || agent.goalSite().match(task.getDelivery())) {
                    
                    flag = false;
                    
                    return true;
                    
                }
                
            }
            
            return false;
            
        });
        
        if(flag){
            
            unsigned distance = system.getEndpointsDistanceAlgorithm().solve(this->currentSite(), task.getPickup());

            if (distance < min_distance) {
                min_distance = distance;
                selectedTask = task;
            }
            
        }
        
        return false;

    });

    return min_distance < 0xffffffff;

}

bool _agent::isConflictingRestEndpoint(_token& token, _task& conflitTask) const {

    bool conflit = false;

    token.listPendingTasks([token, &conflit, &conflitTask, this](const _task & task) {

        if (task.getDelivery().match(this->currentSite())) {

            conflit = true;
            
            conflitTask = task;

            return true;

        }

        return false;

    });

    return conflit;

}

bool _agent::restEndpointPath(const _stepMap& map, const _site& endpoint, _stepPath& restPath) {
    
    _stepAstarAlgorithm astar;

        bool flag = astar.solve(map, restPath, endpoint, this->id());

        if (flag) {

            return true;

        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved rest endpoint path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    return false;


}

bool _agent::taskPath(const _stepMap& map, const _task& task, _stepPath& path) const{
    
    _stepAstarAlgorithm astar;

    bool flag = astar.solve(map, path, task.getPickup(), this->id());

    if (flag) {
        
        flag = astar.solve(map, path, task.getDelivery(), this->id());

        if (flag) {
            
            return true;

        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved task delivery endpoint path: " << task;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else {

        try {
            std::ostringstream stream;
            stream << "unsolved task pickup endpoint path: " << task;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }
    
    return false;

}

void _agent::draw(const Render& render) const{
    
    this->_state->onDraw(render, this);
  
}


