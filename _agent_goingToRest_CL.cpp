/* 
 * File:   _agent_gointToRest_CL.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_goingToRest_CL.h"
#include "_agent_goingToRest.h"
#include "_agent.h"
#include "_agent_dead.h"
#include "_agent_parked.h"
#include "_agent_parked_CL.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_goingToRest_CL::_instance = nullptr;

void _agent_goingToRest_CL::onAfterStepping(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState();
    
    switch(aes){
        
        case AES::charged:
        case AES::normal:
            
            if(agent.isInFinishedPath()){
        
                changeState(agent, _agent_parked::getInstance());       

            }  else {
                
                changeState(agent, _agent_goingToRest::getInstance());
                
            }         
            
            break;
        
        case AES::critical:
            
            if(agent.isInFinishedPath()){
        
                changeState(agent, _agent_parked_CL::getInstance());       

            } 
            
            break;
            
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    } 
    
}

void _agent_goingToRest_CL::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));   
        
}

