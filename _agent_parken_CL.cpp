/* 
 * File:   _agent_parked_CL.cpp
 * Author: ronaldo
 * 
 * Created on 25 de agosto de 2023, 11:41
 */

#include "_agent_parked_CL.h"
#include "_agent.h"
#include "_agent_parked.h"
#include "_agent_goingToPickup_CL.h"
#include "_agent_goingToDelivery_CL.h"
#include "_agent_goingToCharging.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_parked_CL::_instance = nullptr;

void _agent_parked_CL::onUpdatePath(_token& token, _agent& agent) const {
    
    _agent_state::onUpdatePath(token,  agent); // retirar depois
        
    _token::TokenUpdateType tut = token.updateChargingPath(*agent);
    
    switch(tut){
        
        case _token::TokenUpdateType::charging_task:
            
            changeState(agent, _agent_goingToPickup_CL::getInstance());
            
            break;
            
        case _token::TokenUpdateType::charging_rest:
            
            changeState(agent, _agent_goingToRest_CL::getInstance());
            
            break;
            
        case _token::TokenUpdateType::charging_trivial:
            break;
            
        default:
            
            try {
                std::ostringstream stream;
                stream << "invalid token update type: " << agent << std::endl;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
    }   
    
}

void _agent_parked_CL::onBeforeStepping(_token& token, _agent& agent)const {
    
    _agent_state::onBeforeStepping(token,  agent); // retirar depois
    
    if (agent->isPickupping()) { // caso do agente já se encontrar em pickup site

        token.runTask(agent->getCurrentTask());

        changeState(agent, _agent_goingToDelivery_CL::getInstance());

    }
    
}

void _agent_parked_CL::onEnergyExpend(_token& token,  _agent& agent) const {

    _agent_state::onEnergyExpend(token,  agent);
    
    AES aes = AES::none;
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent), aes);
    
    switch(aes){
        case AES::normal:
            changeState(agent, _agent_parked::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_parked_CL::onDraw(const Render& render, const _agent& agent) const {

    sf::Vector2f position(
            agent->currentSite().GetColunm() * render.GetCell().first,
            agent->currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Red);

    Text textAgentId(
            std::to_string(agent->id()),
            position,
            sf::Vector2f(
            render.GetCell().first / 2,
            0),
            sf::Color::White);

    textAgentId.draw(render);

    background.draw(render);
    textAgentId.draw(render);

}