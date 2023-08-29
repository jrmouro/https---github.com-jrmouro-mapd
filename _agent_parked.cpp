/* 
 * File:   _agent_parked.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_parked.h"
#include "_agent.h"
#include "_agent_goingToPickup.h"
#include "_agent_goingToDelivery.h"
#include "_agent_goingToRest.h"
#include "_agent_parked_CL.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_parked::_instance = nullptr;

void _agent_parked::onUpdatePath(_token& token, _agent& agent) const {
    
    _agent_state::onUpdatePath(token,  agent); // retirar depois
        
    _token::TokenUpdateType tut = token.updatePath(*agent);
    
    switch(tut){
        
        case _token::TokenUpdateType::task:
            
            changeState(agent, _agent_goingToPickup::getInstance());
            
            break;
            
        case _token::TokenUpdateType::rest:
            
            changeState(agent, _agent_goingToRest::getInstance());
            
            break;
            
        case _token::TokenUpdateType::trivial:
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

void _agent_parked::onBeforeStepping(_token& token, _agent& agent)const {
    
    _agent_state::onBeforeStepping(token,  agent); // retirar depois
    
    if (agent->isPickupping()) { // caso do agente já se encontrar em pickup site

        token.runTask(agent->getCurrentTask());

        changeState(agent, _agent_goingToDelivery::getInstance());

    }
    
}

void _agent_parked::onEnergyExpend(_token& token,  _agent& agent) const {

    _agent_state::onEnergyExpend(token,  agent);
    
    AES aes = AES::none;
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent), aes);
    
    switch(aes){
        case AES::critical:
            changeState(agent, _agent_parked_CL::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}
