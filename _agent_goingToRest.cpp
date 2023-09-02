/* 
 * File:   _agent_gointToRest.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_goingToRest.h"
#include "_agent_goingToRest_CL.h"
#include "_agent.h"
#include "_agent_parked.h"
#include "_agent_parked_CL.h"
#include "_agent_dead.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToRest::_instance = nullptr;

void _agent_goingToRest::onAfterStepping(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState();
    
    switch(aes){
        
        case AES::charged:
        case AES::normal:
            
            if(agent.isInFinishedPath()){
        
                changeState(agent, _agent_parked::getInstance());       

            }           
            
            break;
        
        case AES::critical:
            
            if(agent.isInFinishedPath()){
        
                changeState(agent, _agent_parked_CL::getInstance());       

            } else {
                
                changeState(agent, _agent_goingToRest_CL::getInstance());
                
            }
            
            break;
            
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
    
}

void _agent_goingToRest::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));
        
}

void _agent_goingToRest::onDraw(const Render& render, const _agent& agent) const {

    _agent_state::onDraw(render, agent);    

    sf::Vector2f position(
            agent.currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
            agent.currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);
    
    Text textTaskId(
            "R",
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Blue);

    textTaskId.draw(render);

}
