/* 
 * File:   _agent_gointToRest.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_goingToCharging.h"
#include "_agent_goingToCharging_CL.h"
#include "_agent.h"
#include "_agent_charging.h"
#include "_agent_charging_CL.h"
#include "_agent_dead.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToCharging::_instance = nullptr;

void _agent_goingToCharging::onAfterStepping(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState();    
        
    switch(aes){
        
        case AES::normal:
        case AES::charged:
            
            if(agent.isInFinishedPath()){
        
                changeState(agent, _agent_charging::getInstance()); 

            }            
            
            break;        
        
        case AES::critical:
            
            if(agent.isInFinishedPath()){
        
                changeState(agent, _agent_charging_CL::getInstance()); 

            } else {
                
                changeState(agent, _agent_goingToCharging_CL::getInstance());
            
            }
            
            break;
            
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_goingToCharging::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));    
    
}

void _agent_goingToCharging::onDraw(const Render& render, const _agent& agent) const {

    _agent_state::onDraw(render, agent);    

    sf::Vector2f position(
            agent.currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
            agent.currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

    Text textTaskId(
            "C",
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Yellow);

    textTaskId.draw(render);

}
