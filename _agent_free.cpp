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

}



