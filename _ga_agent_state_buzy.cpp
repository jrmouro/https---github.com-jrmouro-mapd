/* 
 * File:   _ga_agent_state_buzy.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 08:20
 */

#include "_ga_agent_state_buzy.h"
#include "_ga_agent.h"

_ga_agent_state* _ga_agent_state_buzy::instance = nullptr;

_ga_agent_state* _ga_agent_state_buzy::getInstance(){
        
    if(_ga_agent_state_buzy::instance == nullptr)
        _ga_agent_state_buzy::instance = new _ga_agent_state_buzy();

    return _ga_agent_state_buzy::instance;

}

_ga_agent_state_buzy::_ga_agent_state_buzy(){}

_ga_agent_state_buzy::~_ga_agent_state_buzy(){}

std::string _ga_agent_state_buzy::id() const {
    return "buzy";
}

void _ga_agent_state_buzy::onStepping(const _map& map, _ga_agent& agent) const{
    
    bool inChargingSite = map.getTypeOfSite(agent.getCurrentSite()) == _map::TypeOfSite::bot;
    bool stayedSamePlace = agent.getCurrentSite().match(agent.getFutureSite());
    
    agent.agent_energy_system.expendStepping(inChargingSite, stayedSamePlace, true);
    agent.path.pop();
        
}

