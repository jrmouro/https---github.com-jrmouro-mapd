/* 
 * File:   _agent_gointToRest.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_goingToRest.h"
#include "_agent.h"
#include "_agent_parked.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToRest::_instance = nullptr;

void _agent_goingToRest::onMoveUpdate(_system& system,  _agent* agent)const {
    
    if(agent->previousSite().GetStep() == agent->currentSite().GetStep()){ // parado
        
        agent->expendEnergy(AER::unloaded);
        
    } else { // deslocando
        
        agent->expendEnergy(AER::moving);
        
    }

    if(agent->isParked()){
        
        changeState(agent, _agent_parked::getInstance());
        
    }

}

void _agent_goingToRest::onDraw(const Render& render, const _agent* const agent) const {

    _agent_free::onDraw(render, agent);    

    sf::Vector2f position(
            agent->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
            agent->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

    Text textTaskId(
            "R",
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Yellow);

    textTaskId.draw(render);

}
