/* 
 * File:   _ga_agent_state_free.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 07:42
 */

#include "_ga_agent_state_free.h"
#include "_ga_agent.h"
#include "_ga_agent_state_buzy.h"

_ga_agent_state* _ga_agent_state_free::instance = nullptr;

_ga_agent_state* _ga_agent_state_free::getInstance(){
        
    if(_ga_agent_state_free::instance == nullptr)
        _ga_agent_state_free::instance = new _ga_agent_state_free();

    return _ga_agent_state_free::instance;

}

_ga_agent_state_free::_ga_agent_state_free(){}

_ga_agent_state_free::~_ga_agent_state_free(){}

std::string _ga_agent_state_free::id() const{
    return "free";
}

void _ga_agent_state_free::onStepping(const _map& map, _ga_agent& agent) const{
    
    bool inChargingSite = map.getTypeOfSite(agent.getCurrentSite()) == _map::TypeOfSite::bot;
    bool stayedSamePlace = agent.getCurrentSite().match(agent.getFutureSite());
        
    agent.agent_energy_system.expendStepping(inChargingSite, stayedSamePlace, false);
    agent.path.pop();
    
}