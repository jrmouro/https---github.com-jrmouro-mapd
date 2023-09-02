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
#include "_agent_goingToCharging_CL.h"
#include "_agent_charging_CL.h"
#include "_agent_dead.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_parked_CL::_instance = nullptr;

void _agent_parked_CL::onUpdatePath(_token& token, _agent& agent) const {
        
    _token::TokenUpdateType tut = token.updateChargingPath(agent, true);
    
    switch(tut){
        
        case _token::TokenUpdateType::charging_task:
            
            if(agent.isPickupping()){
        
                token.runTask(agent.currentTask());

                changeState(agent, _agent_goingToDelivery_CL::getInstance());       

            } else {
            
                changeState(agent, _agent_goingToPickup_CL::getInstance());
            
            }
            
            break;
            
        case _token::TokenUpdateType::charging_rest:
            
            changeState(agent, _agent_goingToCharging_CL::getInstance());
            
            break;
            
        case _token::TokenUpdateType::charging_trivial:
            
            changeState(agent, _agent_charging_CL::getInstance());
            
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

void _agent_parked_CL::onEnergyExpend(_token& token,  _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));
        
}

void _agent_parked_CL::onAfterStepping(_token& token,  _agent& agent) const {
    
    AES aes = agent.energyState();
        
    switch(aes){
        case AES::normal:
            changeState(agent, _agent_parked::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}
