/* 
 * File:   _agent_charging.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_charging.h"
#include "_agent.h"
#include "_agent_goingToPickup.h"
#include "_agent_goingToDelivery.h"
#include "_agent_goingToRest.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_charging::_instance = nullptr;

void _agent_charging::onUpdatePath(_token& token, _agent& agent) const{
    
    _agent_state::onUpdatePath(token,  agent); // retirar depois
    
    _token::TokenUpdateType tut = token.updateChargingTrivialPathToAgent(agent, true);
    
    if(tut != _token::TokenUpdateType::charging_trivial){
        
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

void _agent_charging::onAfterStepping(_token& token, _agent& agent) const{
    
    if(agent.isAtEnergyChargingLevel()){
        
        changeState(agent, _agent_parked::getInstance());       //trocar para agent_charging
        
    }
    
}

void _agent_charging::onEnergyExpend(_token& token, _agent& agent) const {
    
    _agent_state::onEnergyExpend(token,  agent);
    
    AES aes = AES::none;
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent), aes);
    
    switch(aes){
        case AES::critical:
            changeState(agent, _agent_charging_CL::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_charging::onDraw(const Render& render, const _agent& agent) const {

    sf::Vector2f position(
            agent.currentSite().GetColunm() * render.GetCell().first,
            agent.currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Black);

    Text textAgentId(
            std::to_string(agent.id()),
            position,
            sf::Vector2f(
            render.GetCell().first / 2,
            0),
            sf::Color::Green);

    textAgentId.draw(render);

    background.draw(render);
    textAgentId.draw(render);    
   

}
