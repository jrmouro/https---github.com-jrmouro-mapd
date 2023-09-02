/* 
 * File:   _agent_goingToPickupCL.cpp
 * Author: ronaldo
 * 
 * Created on 21 de agosto de 2023, 10:06
 */

#include "_agent_goingToPickup_CL.h"
#include "_agent_goingToPickup.h"
#include "_agent.h"
#include "_agent_goingToDelivery.h"
#include "_agent_goingToDelivery_CL.h"
#include "_agent_dead.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToPickup_CL::_instance = nullptr;

void _agent_goingToPickup_CL::onAfterStepping(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState();
    
    switch(aes){
        
        case AES::charged:
        case AES::normal:
            
            if(agent.isPickupping()){
        
                token.runTask(agent.currentTask());

                changeState(agent, _agent_goingToDelivery::getInstance());       

            } else {
                
                changeState(agent, _agent_goingToPickup::getInstance()); 
                
            }
            
            break;
        
        case AES::critical:
            
            if(agent.isPickupping()){
        
                token.runTask(agent.currentTask());

                changeState(agent, _agent_goingToDelivery_CL::getInstance());       

            } 
            
            break;
            
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_goingToPickup_CL::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));
    
}
