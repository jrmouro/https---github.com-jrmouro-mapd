/* 
 * File:   _agent_goingToDelivery.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 17:34
 */

#include "_agent_goingToDelivery.h"
#include "_agent_goingToDelivery_CL.h"
#include "_agent_goingToCharging_CL.h"
#include "_agent.h"
#include "_agent_dead.h"
#include "_agent_parked.h"
#include "_agent_parked_CL.h"
#include "_token.h"
#include "Circle.h"
#include "Text.h"
#include "Render.h"
#include "_system.h"

_agent_state* _agent_goingToDelivery::_instance = nullptr;

void _agent_goingToDelivery::onAfterStepping(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState();
    
    switch(aes){
        
        case AES::normal:
        case AES::charged:
            
            if(agent.isDeliverySite()){
        
                token.finishTask(agent.currentTask());
                
                agent.unassignTask();

                changeState(agent, _agent_parked::getInstance());       

            } else if(agent.isChargingDeliverySite()){

                token.finishTask(agent.currentTask());
                
                agent.unassignTask();

                changeState(agent, _agent_goingToCharging::getInstance());

            } 
            
            break;
        
        case AES::critical:
            
            if(agent.isDeliverySite()){
        
                token.finishTask(agent.currentTask());
                
                agent.unassignTask();

                changeState(agent, _agent_parked_CL::getInstance());       

            } else if(agent.isChargingDeliverySite()){

                token.finishTask(agent.currentTask());
                
                agent.unassignTask();

                changeState(agent, _agent_goingToCharging_CL::getInstance());

            } else {
            
                changeState(agent, _agent_goingToDelivery_CL::getInstance());
            
            }
            
            break;
            
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_goingToDelivery::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendCarryngStepping(token.isChargingSite(agent));   
    
}

void _agent_goingToDelivery::onDraw(const Render& render, const _agent& agent) const {
    
    _agent_state::onDraw(render, agent);
    
    sf::Vector2f position(
            agent.currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
            agent.currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);


    Circle backgroundTask(
            position,
            sf::Vector2f(render.GetCell().first / 4, 0),
            sf::Color::Cyan);
    
    if (agent.isAssigned()) {
        
        Text textTaskId(
                std::to_string(agent.currentTask().id()),
                position,
                sf::Vector2f(render.GetCell().first / 2, 0),
                sf::Color::Black);

        backgroundTask.draw(render);
        textTaskId.draw(render);
        
    }

}
