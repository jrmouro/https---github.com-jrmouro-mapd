/* 
 * File:   _ga_agent_state_buzy.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 08:20
 */

#include "_ga_agent_state_buzy.h"
#include "_ga_agent.h"
#include "Circle.h"
#include "Text.h"
#include "Render.h"

_ga_agent_state* _ga_agent_state_buzy::instance = nullptr;

_ga_agent_state* _ga_agent_state_buzy::getInstance(){
        
    if(_ga_agent_state_buzy::instance == nullptr)
        _ga_agent_state_buzy::instance = new _ga_agent_state_buzy();

    return _ga_agent_state_buzy::instance;

}

_ga_agent_state_buzy::_ga_agent_state_buzy(){}

_ga_agent_state_buzy::~_ga_agent_state_buzy(){}

int _ga_agent_state_buzy::id() const {
    return 1;
}

void _ga_agent_state_buzy::onStepping(const _map& map, _ga_agent& agent) const{
    
    bool inChargingSite = map.getTypeOfSite(agent.currentSite()) == _map::TypeOfSite::bot;
    bool stayedSamePlace = agent.currentSite().match(agent.getFutureSite());
    
    agent.agent_energy_system.expendStepping(inChargingSite, stayedSamePlace, true);
//    agent.path.pop();
        
}

void _ga_agent_state_buzy::onDraw(const Render& render, const _ga_agent& agent) const {

    _ga_agent_state::onDraw(render, agent);
    
    sf::Vector2f position(
            agent.currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
            agent.currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);


    Circle backgroundTask(
            position,
            sf::Vector2f(render.GetCell().first / 4, 0),
            sf::Color::Cyan);
    
        
    Text textTaskId(
            std::to_string(agent.getCurrentTask()->id()),
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Black);

    backgroundTask.draw(render);
    textTaskId.draw(render);
        
    

}

