/* 
 * File:   _agent_dead.cpp
 * Author: ronaldo
 * 
 * Created on 25 de agosto de 2023, 11:28
 */

#include <iostream>
#include "_agent_dead.h"
#include "_agent.h"
#include "_agent_goingToDelivery.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"
#include "MapdException.h"

_agent_state* _agent_dead::_instance = nullptr;

void _agent_dead::onUpdatePath(_system& system,  _agent* agent) const{
    
    try {
            std::ostringstream stream;
            stream << "dead agent: " << std::endl << *agent;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
    
}

void _agent_dead::onAfterStepping(_system& system,  _agent* agent) const{
    
    try {
        std::ostringstream stream;
        stream << "dead agent: " << std::endl << *agent;
        MAPD_EXCEPTION(stream.str());
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::abort();
    }
    
}