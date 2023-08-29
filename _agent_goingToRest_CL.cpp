/* 
 * File:   _agent_gointToRest_CL.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_goingToRest_CL.h"
#include "_agent_goingToRest.h"
#include "_agent.h"
#include "_agent_parked.h"
#include "_agent_parked_CL.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToRest_CL::_instance = nullptr;

void _agent_goingToRest_CL::onAfterStepping(_token& token, _agent& agent) const{
    
    if(agent.isInFinishedPath()){
        
        changeState(agent, _agent_parked_CL::getInstance());       
        
    } 
    
}

void _agent_goingToRest_CL::onEnergyExpend(_token& token, _agent& agent) const {
    
    _agent_state::onEnergyExpend(token,  agent);
    
    AES aes = AES::none;
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent), aes);
    
    switch(aes){
        case AES::normal:
            changeState(agent, _agent_goingToRest::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_goingToRest_CL::onDraw(const Render& render, const _agent& agent) const {

    sf::Vector2f position(
            agent.currentSite().GetColunm() * render.GetCell().first,
            agent.currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Red);

    Text textAgentId(
            std::to_string(agent.id()),
            position,
            sf::Vector2f(
            render.GetCell().first / 2,
            0),
            sf::Color::White);

    textAgentId.draw(render);

    background.draw(render);
    textAgentId.draw(render);   
    
    position += sf::Vector2f(render.GetCell().first / 2, render.GetCell().first / 2);

//    sf::Vector2f position(
//            agent->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
//            agent->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

    Text textTaskId(
            "R",
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Yellow);

    textTaskId.draw(render);

}
