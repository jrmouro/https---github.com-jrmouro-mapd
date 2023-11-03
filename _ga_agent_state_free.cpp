/* 
 * File:   _ga_agent_state_free.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 07:42
 */

#include "_ga_agent_state_free.h"
#include "_ga_agent.h"
#include "_ga_agent_state_buzy.h"
#include "Circle.h"
#include "Text.h"
#include "Render.h"

_ga_agent_state* _ga_agent_state_free::instance = nullptr;

_ga_agent_state* _ga_agent_state_free::getInstance(){
        
    if(_ga_agent_state_free::instance == nullptr)
        _ga_agent_state_free::instance = new _ga_agent_state_free();

    return _ga_agent_state_free::instance;

}

_ga_agent_state_free::_ga_agent_state_free(){}

_ga_agent_state_free::~_ga_agent_state_free(){}

int _ga_agent_state_free::id() const{
    return 0;
}

void _ga_agent_state_free::onStepping(const _map& map, _ga_agent& agent) const{
    
    bool inChargingSite = map.getTypeOfSite(agent.currentSite()) == _map::TypeOfSite::bot;
    bool stayedSamePlace = agent.currentSite().match(agent.getFutureSite());
        
    agent.agent_energy_system.expendStepping(inChargingSite, stayedSamePlace, false);
//    agent.path.pop();
    
}

void _ga_agent_state_free::onDraw(const Render& render, const _ga_agent& agent) const {

    _ga_agent_state::onDraw(render, agent);
    
    if(agent.isTaskAssigned()){
    
        sf::Vector2f position(
                agent.currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
                agent.currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

        Text textTaskId(
                std::to_string(agent.getCurrentTask()->id()),
                position,
                sf::Vector2f(render.GetCell().first / 2, 0),
                sf::Color::Cyan);

        textTaskId.draw(render);
        
    }

}