/* 
 * File:   _agent_gointToCharging_CL.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_goingToCharging.h"
#include "_agent.h"
#include "_agent_parked.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToCharging_CL::_instance = nullptr;

void _agent_goingToCharging_CL::onAfterStepping(_token& token, _agent& agent) const{
    
    if(agent.isInFinishedPath()){
        
        changeState(agent, _agent_charging_CL::getInstance());       //trocar para agent_charging
        
    }
    
}

void _agent_goingToCharging_CL::onEnergyExpend(_token& token, _agent& agent) const {
    
    _agent_state::onEnergyExpend(token,  agent);
    
    AES aes = AES::none;
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent), aes);
    
    switch(aes){
        case AES::normal:
            changeState(agent, _agent_goingToCharging::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_goingToCharging::onDraw(const Render& render, const _agent* const agent) const {

    _agent_state::onDraw(render, agent);    

    sf::Vector2f position(
            agent->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
            agent->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

    Text textTaskId(
            "C",
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Yellow);

    textTaskId.draw(render);

}
