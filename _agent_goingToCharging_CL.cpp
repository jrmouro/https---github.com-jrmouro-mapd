/* 
 * File:   _agent_gointToCharging_CL.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_goingToCharging_CL.h"
#include "_agent_goingToCharging.h"
#include "_agent_charging.h"
#include "_agent_charging_CL.h"
#include "_agent.h"
#include "_agent_dead.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToCharging_CL::_instance = nullptr;

void _agent_goingToCharging_CL::onAfterStepping(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState();    
        
    switch(aes){
        
        case AES::normal:
        case AES::charged:
            
            if(agent.isInGoalSite()){
        
                changeState(agent, _agent_charging::getInstance()); 

            } else {
                
                changeState(agent, _agent_goingToCharging::getInstance());
                
            }       
            
            break;        
        
        case AES::critical:
            
            if(agent.isInGoalSite()){
        
                changeState(agent, _agent_charging_CL::getInstance());       

            } 
            
            break;
            
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_goingToCharging_CL::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));
    
}
