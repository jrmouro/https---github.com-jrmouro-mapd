/* 
 * File:   _agent_occupied.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 17:34
 */

#include "_agent_occupied.h"
#include "_agent.h"
#include "_agent_free.h"
#include "_token.h"
#include "Circle.h"
#include "Text.h"
#include "Render.h"
#include "_system.h"

_agent_state* _agent_occupied::_instance = nullptr;

void _agent_occupied::onMoveUpdate(_system& system,  _agent* agent)const {

   if (agent->isDelivering()){
       
       system.getToken().finishTask(agent->getCurrentTask());

       agent->undesignTask();
       
       changeState(agent, _agent_free::getInstance());
       
   }

}

void _agent_occupied::onDraw(const Render& render, const _agent* const agent) const {
    
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

    background.draw(render);
    textAgentId.draw(render);
    
    position += sf::Vector2f(render.GetCell().first / 2, render.GetCell().first / 2);

    Circle backgroundTask(
            position,
            sf::Vector2f(render.GetCell().first / 4, 0),
            sf::Color::Cyan);
    
    if (agent->isDesigned()) {
        
        Text textTaskId(
                std::to_string(agent->getCurrentTask().id()),
                position,
                sf::Vector2f(render.GetCell().first / 2, 0),
                sf::Color::Black);

        backgroundTask.draw(render);
        textTaskId.draw(render);
        
    }

}
