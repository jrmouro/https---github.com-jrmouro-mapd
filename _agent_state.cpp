/* 
 * File:   _agent_state.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 10:39
 */

#include "_agent_state.h"
#include "Circle.h"
#include "Text.h"
#include "Render.h"
#include "_agent.h"

void _agent_state::onUpdatePath(_token& token,  _agent& agent) const {

//    if(!agent.isInFinishedPath()){
//        
//        try {
//            std::ostringstream stream;
//            stream << "agent is not in finished path: " << agent << std::endl;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//        
//    } 
    
    // ---> derive here or replace

}

void _agent_state::onBeforeStepping(_token& token,  _agent& agent) const {
   
//    if(agent.isInFinishedPath()){
//        
//        try {
//            std::ostringstream stream;
//            stream << "agent is in finished path: " << agent << std::endl;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//        
//    } 
    
    // ---> derive here        

}

void _agent_state::onStepping(_token& token,  _agent& agent) const {
    
    agent.stepping();

}

void _agent_state::onAfterStepping(_token& token,  _agent& agent) const {}


void _agent_state::onDraw(const Render& render, const _agent& agent) const {
    
    sf::Color energy = sf::Color::Green;
    if(agent.isAtEnergyChargedLevel()) {
        energy = sf::Color::Blue;
    } else if(agent.isAtEnergyCriticalLevel()){
        energy = sf::Color::Yellow;
    } else if(agent.isAtEnergyDeadLevel()){
        energy = sf::Color::Red;
    }
    

    sf::Vector2f position(
            agent.currentSite().GetColunm() * render.GetCell().first,
            agent.currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Black);
    
    Circle backgroundId(
            position,
            sf::Vector2f(render.GetCell().first / 4, 0),
            energy);

    Text textAgentId(
            std::to_string(agent.id()),
            position,
            sf::Vector2f(
            render.GetCell().first / 2,
            0),
            sf::Color::Black);

    textAgentId.draw(render);

    background.draw(render);
    backgroundId.draw(render);
    textAgentId.draw(render);
    
    // ---> derive here

}

void _agent_state::changeState(_agent& agent, _agent_state* state) const{
    agent.changeState(state);
}

