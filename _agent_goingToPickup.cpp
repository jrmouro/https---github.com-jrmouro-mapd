/* 
 * File:   _agent_designed.cpp
 * Author: ronaldo
 * 
 * Created on 21 de agosto de 2023, 10:06
 */

#include "_agent_designed.h"
#include "_agent.h"
#include "_agent_occupied.h"
#include "_agent_parked.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_designed::_instance = nullptr;

void _agent_designed::onMoveUpdate(_system& system,  _agent* agent)const {
    
    if(agent->previousSite().GetStep() == agent->currentSite().GetStep()){ // parado
        
        agent->expendEnergy(AER::unloaded);
        
    } else { // deslocando
        
        agent->expendEnergy(AER::moving);
        
    }

    if (agent->isPickupping()) {
        
        system.getToken().runTask(agent->getCurrentTask());

        changeState(agent, _agent_occupied::getInstance());

    }
    
    if(agent->isParked()){
        
        changeState(agent, _agent_parked::getInstance());
        
    }

}

void _agent_designed::onDraw(const Render& render, const _agent* const agent) const {

    _agent_state::onDraw(render, agent);    

    sf::Vector2f position(
            agent->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
            agent->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

    Text textTaskId(
            std::to_string(agent->getCurrentTask().id()),
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Cyan);

    textTaskId.draw(render);

}