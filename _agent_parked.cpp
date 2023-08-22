/* 
 * File:   _agent_parked.cpp
 * Author: ronaldo
 * 
 * Created on 22 de agosto de 2023, 02:55
 */

#include "_agent_parked.h"
#include "_agent.h"
#include "_agent_designed.h"
#include "_agent_occupied.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_parked::_instance = nullptr;

void _agent_parked::onUpdatePath(_system& system,  _agent* agent) const {
    
//    agent->updatePath(system);
    
    system.getToken().updatePath(*agent);
    
    if(agent->isDesigned()){
        
        changeState(agent, _agent_designed::getInstance());
        
    }
    
    if (agent->isPickupping()) { // caso do agente já se encontrar em pickup site

        system.getToken().runTask(agent->getCurrentTask());
        
        changeState(agent, _agent_occupied::getInstance());

    } 
    
}

void _agent_parked::onMoveUpdate(_system& system,  _agent* agent)const {

    

}
