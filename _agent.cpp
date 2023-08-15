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

_agent::_agent(int _id, const _stepSite& currentSite) : 
_id(_id), _currentPath(currentSite) { 

    this->_state = new _agent_free(this);

}

_agent::_agent(const _agent& other) :  _id(other._id), _currentPath(other._currentPath) {

    this->_state = other._state->builder(other._state->stateId(), this);
}


_agent::~_agent(){ 
    if(_state != nullptr)
        delete _state;
}

void _agent::receive(_token& token) {
 
    this->_state->onUpdatePath(token);
    
}

void _agent::move(_token& token) {


//        _currentPath.pop();
    this->stepping();
        
    this->_state->onMoveUpdate(token);

    

}


//void _agent::updatePath(Token& token){
//
//    if (state == free) {
//
//        if (this->isResting()) {
//
//            if (selectNewTask(token)) {
//
//                updateTaskPath(token);
//                state == occupied;
//
//            } else if (isParked()) {
//
//                if (isConflictingRestEndpoint(token)) {
//
//                    updateRestEndpointPath(token);
//
//                } else {
//
//                    updateTrivialPath(token);
//
//                }
//
//            }
//
//        }
//        
//    }
//
//}


//void _agent::moveUpdate(Token& token) {
//    
//    if (state == free) {
//
//        if(this->isPickupping()){
//            
//            this->state = occupied;
//            
//        }        
//
//    } else if (state == occupied) {
//
//        if(this->isDelivering()){
//            
//            token.removeOpenTask(*currentTask);
//            delete currentTask;
//            currentTask = nullptr;
//            this->state = free;
//            
//        }
//
//    }    
//
//}

void _agent::updateTrivialPath(_token& token) {

    if (_currentPath.size() == 1) {

        _stepSite site = _currentPath.currentSite();
        site.SetStep(site.GetStep() + 1);
        _currentPath.progress(site);
        token.getIntegerMap().setMoving(_currentPath, this->id());

    } else {

        try {
            std::ostringstream stream;
            stream << "unsolved trivial path";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

}

_site* _agent::selectNewEndpoint(_token& token) {

    unsigned min_distance = 0xffffffff;
    _site* endpoint = nullptr;

    token.listEndpoints([&token, &min_distance, &endpoint, this](const _site & site) {

        token.listTasks([&token, &min_distance, &endpoint, site, this](const Task & task) {

            if (!task.getDelivery().match(site)) {

                token.listAgents([&token, &min_distance, &endpoint, site, task, this](_agent & agent) {

                    if (this->id() != agent.id()) { //other agents

                        if (!agent.goalSite().match(site)) {

                            unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), site);

                            if (distance < min_distance) {
                                min_distance = distance;
                                if (endpoint != nullptr) delete endpoint;
                                endpoint = new _site(site);
                            }

                        }

                    } 
                    
//                    else {
//
//                        unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), site);
//
//                        if (distance < min_distance) {
//                            min_distance = distance;
//                            if (endpoint != nullptr) delete endpoint;
//                            endpoint = new _site(site);
//                        }
//
//                    }

                    return false;

                });


            }

            return false;

        });


        return false;

    });

    return endpoint;

}

bool _agent::selectNewTask(_token& token) {


    unsigned min_distance = 0xffffffff;
    Task selectedTask;

    token.listTasks([&token, &min_distance, &selectedTask, this](const Task & task) {
        
        bool flag = true;
        
        token.listAgents([&token, &min_distance, &selectedTask, task, this, &flag](const _agent & agent) {
            
            if (this->id() != agent.id()) {
                
                if (agent.goalSite().match(task.getPickup()) || agent.goalSite().match(task.getDelivery())) {
                    
                    flag = false;
                    
                    return true;
                    
                }
                
            }
            
            return false;
            
        });
        
        if(flag){
            
            unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), task.getPickup());

            if (distance < min_distance) {
                min_distance = distance;
                selectedTask = task;
            }
            
        }
        

//        token.listAgents([&token, &min_distance, &selectedTask, task, this](const _agent & agent) {
//
//            if (this->id() != agent.id()) { //other agents
//
//                if (!agent.goalSite().match(task.getPickup()) && !agent.goalSite().match(task.getDelivery())) {
//
//                    unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), task.getPickup());
//
//                    if (distance < min_distance) {
//                        min_distance = distance;
//                        selectedTask = task;
//                    }
//
//                }
//
//            }
//
//            return false;
//
//        });

        return false;

    });

    if (min_distance < 0xffffffff) {

        if (_currentTask != nullptr) delete _currentTask;
        _currentTask = new Task(selectedTask);

        token.removePendingTask(selectedTask);
        token.addOpenTask(selectedTask);

        return true;

    }

    return false;

}

bool _agent::isConflictingRestEndpoint(_token& token, Task* const conflitTask) const {

    bool conflit = false;

    token.listTasks([token, &conflit, conflitTask, this](const Task & task) {

        if (task.getDelivery().match(this->currentSite())) {

            conflit = true;
            
            *conflitTask = task;

            return true;

        }

        return false;

    });

    return conflit;

}

void _agent::updateRestEndpointPath(_token& token, const Task& conflitTask) {

    _site* endpoint = selectNewEndpoint(token);

    if (endpoint != nullptr) {

        _stepPath restPath;

        bool flag = token.pathAlgorithm()->solve(
                token.getIntegerMap(),
                this->_currentPath.goalSite(),
                *endpoint,
                restPath,
                this->_currentPath.goalSite().GetStep(), this->id());


        delete endpoint;

        if (flag) {

            restPath.pop();

            if (restPath.size() > 0) {

                token.reportTaskUpdate(conflitTask.id(), this->id(), ReportTask::PathType::rest, restPath);
                _currentPath.progress(restPath);
                token.getIntegerMap().setMoving(_currentPath, this->id());


            }

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

    } else {

        try {
            std::ostringstream stream;
             stream << "new rest endpoint not found";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }


}


void _agent::updateTaskPath(_token& token) {

    _stepPath pickupPath;
    bool flag = token.pathAlgorithm()->solve(
            token.getIntegerMap(),
            this->_currentPath.currentSite(),
            _currentTask->getPickup(),
            pickupPath,
            token.getCurrentStep(), this->id());

    if (flag) {

        token.reportTaskUpdate(_currentTask->id(), this->id(), ReportTask::PathType::pickup, pickupPath);
        pickupPath.pop();
        _currentPath.progress(pickupPath);

        _stepPath deliveryPath;
        flag = token.pathAlgorithm()->solve(
                token.getIntegerMap(),
                _currentTask->getPickup(),
                _currentTask->getDelivery(),
                deliveryPath,
                token.getCurrentStep() + _currentPath.size() - 1, this->id());

        if (flag) {

            token.reportTaskUpdate(_currentTask->id(), this->id(), ReportTask::PathType::delivery, deliveryPath);

            deliveryPath.pop();
            _currentPath.progress(deliveryPath);
            token.getIntegerMap().setMoving(_currentPath, this->id());


        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved task delivery endpoint path: " << *_currentTask;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else {

        try {
            std::ostringstream stream;
            stream << "unsolved task pickup endpoint path: " << *_currentTask;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

}


//void Agent::draw(const Render& render) const {
//
//    sf::Vector2f position(
//            this->currentSite().GetColunm() * render.GetCell().first,
//            this->currentSite().GetRow() * render.GetCell().second);
//
//    Circle background(
//            position,
//            sf::Vector2f(render.GetCell().first / 2, 0),
//            sf::Color::Black);
//
//    Text textAgentId(
//            std::to_string(this->id()),
//            position,
//            sf::Vector2f(
//            render.GetCell().first / 2,
//            0),
//            sf::Color::White);
//
//    textAgentId.draw(render);
//
//    background.draw(render);
//    textAgentId.draw(render);
//
//    if (this->currentTask != nullptr) {
//
//        Text textTaskId(
//                std::to_string(currentTask->id()),
//                position + sf::Vector2f(render.GetCell().first / 2, render.GetCell().first / 2),
//                sf::Vector2f(
//                render.GetCell().first / 2,
//                0),
//                sf::Color::Cyan);
//
//        textTaskId.draw(render);
//
//        if (this->state == occupied) {
//
//            Circle occupied(
//                    position + sf::Vector2f(render.GetCell().first / 2, 0),
//                    sf::Vector2f(render.GetCell().first / 4, 0),
//                    sf::Color::Cyan);
//
//            occupied.draw(render);
//
//
//        }
//
//    }
//
//
//
//}
//
//
//
//
//
//
