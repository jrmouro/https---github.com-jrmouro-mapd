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

_agent::_agent(int _id, const _stepSite& currentSite) : 
_id(_id), _currentPath(currentSite), _state(new _agent_free()) { }

_agent::_agent(const _agent& other)  :
    _id(other._id), 
    _state(other._state->builder()), 
    _currentPath(other._currentPath), 
    tasks(other.tasks), 
    _currentTaskIndex(other._currentTaskIndex) { }


_agent::~_agent(){ 
    if(_state != nullptr)
        delete _state;
}

void _agent::receive(_token& token) {
 
//    this->_state->onUpdatePath(token, this);
    
//    std::cout << "aqui";
    
    _task task;
    
    if (this->isParked()) { // request token condition
        
        if(this->isPickupping()){
            
        }
        
        if(this->isDesigned()){
            
            token.removeOpenTask(this->getCurrentTask());

            this->undesignTask();
        }
                   
        if (this->selectNewTask(token, task)) {
            
            this->designTask(task);
            
            token.removePendingTask(task);
            token.addOpenTask(task);

            this->updateTaskPath(token, task);
            
            if (this->isParked()) {
                
                token.removeOpenTask(task);

                this->undesignTask();
                
                this->updateTrivialPath(token); 
                
            } 
            
            return;

        } else if (this->isConflictingRestEndpoint(token, task)) {

            this->updateRestEndpointPath(token, task);

            return;

        }
        
        this->updateTrivialPath(token);       
        
    }
    
}

void _agent::move(_token& token) {
    
    

    this->stepping();
        
//    this->_state->onMoveUpdate(token, this);

}

void _agent::updateTrivialPath(_token& token) {

    if (_currentPath.size() == 1) {

        _stepSite site = _currentPath.currentSite();
        site.SetStep(site.GetStep() + 1);
        _currentPath.progress(site);
        token.getStepMap().setMoving(_currentPath, this->id());

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

bool _agent::selectNewEndpoint(_token& token, _site& selectNewSite) {

    unsigned min_distance = 0xffffffff;

    token.listEndpoints([&token, &min_distance, &selectNewSite, this](const _site &endpoint) {
        
        bool flag = true;

        token.listTasks([endpoint, &flag](const _task & task) {

            if (task.getDelivery().match(endpoint)) {

                flag = false;
                
                return true;                

            }

            return false;

        });
        
        if(flag){
        
            token.listAgents([endpoint, this, &flag](_agent & agent) {

                if (this->id() != agent.id() && agent.goalSite().match(endpoint)) { //other agents

                    flag = false;
                    
                    return true;

                } 

                return false;

            });
                
        }
        
        if(flag){
            
//            unsigned distance = token.getManhattanAlgorithm().solve(this->currentSite(), endpoint);
            unsigned distance = token.getEndpointsDistanceAlgorithm().solve(this->currentSite(), endpoint);
            
            if (distance < min_distance) {
                min_distance = distance;
                selectNewSite = endpoint;
            }
            
            
        }

        return false;

    });

    return min_distance < 0xffffffff;

}

bool _agent::selectNewTask(_token& token, _task& selectedTask) {

    unsigned min_distance = 0xffffffff;
    
    token.listTasks([&token, &min_distance, &selectedTask, this](const _task & task) {
        
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
            
//            unsigned distance = token.getManhattanAlgorithm().solve(this->currentSite(), task.getPickup());
            unsigned distance = token.getEndpointsDistanceAlgorithm().solve(this->currentSite(), task.getPickup());

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

    token.listTasks([token, &conflit, &conflitTask, this](const _task & task) {

        if (task.getDelivery().match(this->currentSite())) {

            conflit = true;
            
            conflitTask = task;

            return true;

        }

        return false;

    });

    return conflit;

}

void _agent::updateRestEndpointPath(_token& token, const _task& conflitTask) {

    _site endpoint;

    if (selectNewEndpoint(token, endpoint)) {

        _stepPath restPath;

        bool flag = token.getStepAstarAlgorithm().solve(
                token.getStepMap(),
                this->_currentPath.goalSite(),
                endpoint,
                restPath,
                this->_currentPath.goalSite().GetStep(), this->id());

        if (flag) {

            restPath.pop();

            if (restPath.size() > 0) {

                token.reportTaskUpdate(conflitTask.id(), this->id(), ReportTask::PathType::rest, restPath);
                _currentPath.progress(restPath);
                token.getStepMap().setMoving(_currentPath, this->id());


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


void _agent::updateTaskPath(_token& token, const _task& task) {

    _stepPath pickupPath;
    bool flag = token.getStepAstarAlgorithm().solve(
            token.getStepMap(),
            this->_currentPath.currentSite(),
            task.getPickup(),
            pickupPath,
            token.getCurrentStep(), this->id());

    if (flag) {

        token.reportTaskUpdate(task.id(), this->id(), ReportTask::PathType::pickup, pickupPath);
        pickupPath.pop();
        _currentPath.progress(pickupPath);

        _stepPath deliveryPath;
        flag = token.getStepAstarAlgorithm().solve(
                token.getStepMap(),
                task.getPickup(),
                task.getDelivery(),
                deliveryPath,
                token.getCurrentStep() + _currentPath.size() - 1, this->id());

        if (flag) {

            token.reportTaskUpdate(task.id(), this->id(), ReportTask::PathType::delivery, deliveryPath);

            deliveryPath.pop();
            _currentPath.progress(deliveryPath);
            token.getStepMap().setMoving(_currentPath, this->id());


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

}

void _agent::draw(const Render& render) const{
    
//    this->_state->onDraw(render, this);
    
    sf::Vector2f position(
            this->currentSite().GetColunm() * render.GetCell().first,
            this->currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color(this->id()*100, this->id()*100, this->id()*100));

    Text textAgentId(
            std::to_string(this->id()),
            position,
            sf::Vector2f(
            render.GetCell().first / 2,
            0),
            sf::Color::White);

    background.draw(render);
    textAgentId.draw(render);

    if (this->isDesigned()) {

        sf::Vector2f position(
                this->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
                this->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

        Text textTaskId(
                std::to_string(this->getCurrentTask().id()),
                position,
                sf::Vector2f(render.GetCell().first / 2, 0),
                sf::Color::Cyan);

        textTaskId.draw(render);

    }
    
}


