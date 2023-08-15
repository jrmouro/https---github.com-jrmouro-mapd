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

void _agent_occupied::onMoveUpdate(_token& token)const {

    if (this->agent->isDelivering()) {

        token.removeOpenTask(*this->agent->currentTask());
        this->agent->undesignTask();

        this->agent->setState(new _agent_free(this->agent));

    }

}

void _agent_occupied::onDraw(const Render& render) const {
    
    sf::Vector2f position(
            this->agent->currentSite().GetColunm() * render.GetCell().first,
            this->agent->currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Black);

    Text textAgentId(
            std::to_string(this->agent->id()),
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
    
    
    Text textTaskId(
            std::to_string(this->agent->currentTask()->id()),
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Black);

    backgroundTask.draw(render);
    textTaskId.draw(render);

}
