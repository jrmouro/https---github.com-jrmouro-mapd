/* 
 * File:   _agent.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 09:30
 */

#include "_agent.h"
#include "_agent_state.h"
#include "_token.h"
#include "_agent_parked.h"
#include "_agent_parked_CL.h"
#include "_agent_charging.h"
#include "_agent_charging_CL.h"
#include "_agent_goingToPickup.h"
#include "_agent_goingToPickup_CL.h"
#include "_agent_goingToCharging_CL.h"
#include "_agent_goingToRest.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent::_agent(int _id, const _stepSite& currentSite, const _agent_energy_system& agent_energy_system): 
                _id(_id), 
                _currentPath(currentSite), 
                _previousSite(currentSite),
                energy_system(agent_energy_system) {

    if(energy_system.isAtCriticalLevel()) {
        
        this->changeState(_agent_charging_CL::getInstance());
        
    } else {
        
        this->changeState(_agent_charging::getInstance());
        
    }

}

_agent::_agent(const _agent& other)  :
    _id(other._id), 
    _state(other._state), 
    _currentPath(other._currentPath), 
    tasks(other.tasks), 
    _currentTaskIndex(other._currentTaskIndex),
    energy_system(other.energy_system),
    _previousSite(other._previousSite){ }


_agent::~_agent(){ }

void _agent::receive(_token& token) {
 
    this->_state->onUpdatePath(token, *this);
        
}

void _agent::expendEnergy(_token& token) {
        
    this->_state->onEnergyExpend(token, *this);

}

void _agent::stepping(_token& token) {
        
    this->_state->onStepping(token, *this);

}

void _agent::move(_token& token) {
    
    this->_state->onEnergyExpend(token, *this);
    
//    this->_state->onBeforeStepping(token, *this);
        
    this->_state->onStepping(token, *this);
     
    this->_state->onAfterStepping(token, *this);
    
}

void _agent::draw(const Render& render) const{
    
    this->_state->onDraw(render, *this);
  
}

void _agent::setPathSwap(const _stepPath& path){
        
    setPath(path);
    
    if(path.isTrivial()){
        
        if(energy_system.isAtCriticalLevel()) {

            this->changeState(_agent_parked_CL::getInstance());

        } else {

            this->changeState(_agent_parked::getInstance());

        }         
        
    }else{

        if(energy_system.isAtCriticalLevel()) {

            this->changeState(_agent_goingToCharging_CL::getInstance());

        } else {

            this->changeState(_agent_goingToRest::getInstance());

        }
    
    }

}

void _agent::assignTaskSwap(const _task& task, const _stepPath& path){
        
    assignTask(task, path);
    
    if(path.isTrivial()){
        
        unassignTaskSwap(task);
        
        if(energy_system.isAtCriticalLevel()) {

            this->changeState(_agent_parked_CL::getInstance());

        } else {

            this->changeState(_agent_parked::getInstance());

        }
        
    }else{

        if(energy_system.isAtCriticalLevel()) {

            this->changeState(_agent_goingToPickup_CL::getInstance());

        } else {

            this->changeState(_agent_goingToPickup::getInstance());

        }
    
    }

}

void _agent::unassignTaskSwap(const _task& task){
    
    if(isAssigned()){
        
        if(task.id() == tasks.back().id()){

            setPath(_stepPath(currentSite()));
            _currentTaskIndex = -1;
            tasks.pop_back();

            if(energy_system.isAtCriticalLevel()) {

                this->changeState(_agent_parked_CL::getInstance());

            } else {

                this->changeState(_agent_parked::getInstance());

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "unrecognized task as assigned: " << task;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        } 
    
    } else {
        
        try {
            std::ostringstream stream;
            stream << "agent is not assigned to any task: " << *this;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

}


