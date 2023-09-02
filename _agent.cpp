/* 
 * File:   _agent.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 09:30
 */

#include "_agent.h"
#include "_agent_state.h"
#include "_token.h"
#include "_agent_charging.h"
#include "_agent_charging_CL.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent::_agent(
        int _id, 
        const _stepSite& currentSite, 
        int energyCurrentLevel, 
        int energyMaximumLevel,
        int energyChargingLevel,  
        int energyCriticalLevel) : 
                _id(_id), 
                _currentPath(currentSite), 
                _previousSite(currentSite),
                energy_system(
                    energyCurrentLevel, 
                    energyMaximumLevel,
                    energyChargingLevel, 
                    energyCriticalLevel) {

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

void _agent::receive(_system& system) {
 
    this->_state->onUpdatePath(system.getToken(), *this);
        
}

void _agent::expendEnergy(_token& token) {
        
    this->_state->onEnergyExpend(token, *this);

}

void _agent::stepping(_token& token) {
        
    this->_state->onStepping(token, *this);

}

void _agent::move(_system& system) {
    
    this->_state->onBeforeStepping(system.getToken(), *this);
    
    this->_state->onEnergyExpend(system.getToken(), *this);
    
    this->_state->onStepping(system.getToken(), *this);
     
    this->_state->onAfterStepping(system.getToken(), *this);
    
}

void _agent::draw(const Render& render) const{
    
    this->_state->onDraw(render, *this);
  
}


