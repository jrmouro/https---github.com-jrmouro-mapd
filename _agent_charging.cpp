/* 
 * File:   _agent_charging.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_charging.h"
#include "_agent.h"
#include "_agent_parked.h"
#include "_agent_charging_CL.h"
#include "_agent_goingToPickup.h"
#include "_agent_goingToDelivery.h"
#include "_agent_goingToRest.h"
#include "_agent_dead.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_charging::_instance = nullptr;

void _agent_charging::onUpdatePath(_token& token, _agent& agent) const{
    
    AES aes = agent.energyState(); 
    
    _token::TokenUpdateType tut;
    
    switch(aes){
        
        case AES::charged:
                        
            tut = token.updatePath(agent);
    
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
                    
                    changeState(agent, _agent_parked::getInstance());   
                    
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
            
            
            break;
            
        case AES::critical:
            changeState(agent, _agent_charging_CL::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
                
        default:   
    
            tut = token.updateTrivialPathToAgent(agent);

            if(tut != _token::TokenUpdateType::trivial){

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
    
}

void _agent_charging::onAfterStepping(_token& token, _agent& agent) const {
    
    AES aes = agent.energyState();   
    
    switch(aes){
        case AES::charged:
            changeState(agent, _agent_parked::getInstance());
            break;
        case AES::critical:
            changeState(agent, _agent_charging_CL::getInstance());
            break;
        case AES::dead:
            changeState(agent, _agent_dead::getInstance());
            break;
    }
    
}

void _agent_charging::onEnergyExpend(_token& token, _agent& agent) const {
    
    agent.expendNoCarryngStepping(token.isChargingSite(agent));   
    
}

void _agent_charging::onDraw(const Render& render, const _agent& agent) const {
    
    _agent_state::onDraw(render, agent);   

}
