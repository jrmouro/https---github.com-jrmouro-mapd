/* 
 * File:   _agent_goingToDelivery_CL.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 17:34
 */

#include "_agent_goingToDelivery_CL.h"
#include "_agent_goingToDelivery.h"
#include "_agent.h"
#include "_agent_goingToCharging_CL.h"
#include "_agent_dead.h"
#include "_agent_parked_CL.h"
#include "_token.h"
#include "Circle.h"
#include "Text.h"
#include "Render.h"
#include "_system.h"

_agent_state* _agent_goingToDelivery_CL::_instance = nullptr;

void _agent_goingToDelivery_CL::onAfterStepping(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState();
    
    switch(aes){
        
        case AES::normal:
        case AES::charged:
            
            if(agent.isDeliverySite()){
        
                token.finishTask(agent.currentTask());
                
                agent.finishTask();

                changeState(agent, _agent_parked::getInstance());       

            } else if(agent.isChargingDeliverySite()){

                token.finishTask(agent.currentTask());
                
                agent.finishTask();

                changeState(agent, _agent_goingToCharging::getInstance());

            } else {
                
                changeState(agent, _agent_goingToDelivery::getInstance());
                
            }
            
            break;
        
        case AES::critical:
            
            if(agent.isDeliverySite()){
        
                token.finishTask(agent.currentTask());
                
                agent.finishTask();

                changeState(agent, _agent_parked_CL::getInstance());       

            } else if(agent.isChargingDeliverySite()){

                token.finishTask(agent.currentTask());
                
                agent.finishTask();

                changeState(agent, _agent_goingToCharging_CL::getInstance());

            } 
            
            break;
            
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_goingToDelivery_CL::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendCarryngStepping(token.isChargingSite(agent));   
    
}
