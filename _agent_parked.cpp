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
#include "_agent_dead.h"
#include "_agent_goingToRest.h"
#include "_agent_parked_CL.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_parked::_instance = nullptr;

void _agent_parked::onUpdatePath(_token& token, _agent& agent) const {
        
    _token::TokenUpdateType tut = token.updatePath(agent);
    
    switch(tut){
        
        case _token::TokenUpdateType::task:
            
            if(agent.isPickupSite()){
        
                token.runTask(agent.currentTask());

                changeState(agent, _agent_goingToDelivery::getInstance());       

            } else {
            
                changeState(agent, _agent_goingToPickup::getInstance());
            
            }
            
            break;
            
        case _token::TokenUpdateType::rest:
            
            changeState(agent, _agent_goingToRest::getInstance());
            
            break;
            
        case _token::TokenUpdateType::trivial:
            break;
            
        case _token::TokenUpdateType::none:
            
            changeState(agent, _agent_dead::getInstance());
            
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

void _agent_parked::onEnergyExpend(_token& token,  _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));
    
}

void _agent_parked::onAfterStepping(_token& token,  _agent& agent) const {
    
    AES aes = agent.energyState();
        
    switch(aes){
        case AES::critical:
            changeState(agent, _agent_parked_CL::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}
