/* 
 * File:   _agent_free.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 14:28
 */

#include "_agent_free.h"
#include "_agent.h"
#include "_agent_occupied.h"
#include "_token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"
#include "_system.h"

_agent_state* _agent_free::_instance = nullptr;

void _agent_free::onUpdatePath(_system& system,  _agent* agent) const {
    
    agent->updatePath(system);
    
    if (agent->isPickupping()) {

        system.getToken().runTask(agent->getCurrentTask());
        
        changeState(agent, _agent_occupied::getInstance());

    }
    
}

void _agent_free::onMoveUpdate(_system& system,  _agent* agent)const {

    if (agent->isPickupping()) {
        
        system.getToken().runTask(agent->getCurrentTask());

        changeState(agent, _agent_occupied::getInstance());

    }

}

void _agent_free::onDraw(const Render& render, const _agent* const agent) const {

    sf::Vector2f position(
            agent->currentSite().GetColunm() * render.GetCell().first,
            agent->currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Black);

    Text textAgentId(
            std::to_string(agent->id()),
            position,
            sf::Vector2f(
            render.GetCell().first / 2,
            0),
            sf::Color::White);

    textAgentId.draw(render);

    background.draw(render);
    textAgentId.draw(render);

//    if (agent->isDesigned()) {
//
//        sf::Vector2f position(
//                agent->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
//                agent->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);
//
//        Text textTaskId(
//                std::to_string(agent->getCurrentTask().id()),
//                position,
//                sf::Vector2f(render.GetCell().first / 2, 0),
//                sf::Color::Cyan);
//
//        textTaskId.draw(render);
//
//    }

}



