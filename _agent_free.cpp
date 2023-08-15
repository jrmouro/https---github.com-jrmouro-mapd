/* 
 * File:   _agent_free.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 14:28
 */

#include "_agent_free.h"
#include "_agent.h"
#include "_agent_occupied.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"

void _agent_free::onUpdatePath(_token& token)const {

    if (this->agent->isResting()) {

        if (this->agent->selectNewTask(token)) {

            this->agent->updateTaskPath(token);

            if (this->agent->isPickupping()) {

                this->agent->setState(new _agent_occupied(this->agent));

            }

            return;

        }

        Task conflit;

        if (this->agent->isConflictingRestEndpoint(token, &conflit)) {

            this->agent->updateRestEndpointPath(token, conflit);

            return;

        }

    }

    if (this->agent->isParked()) {

        Task conflit;

        if (this->agent->isConflictingRestEndpoint(token, &conflit)) {

            this->agent->updateRestEndpointPath(token, conflit);

            return;

        }

        this->agent->updateTrivialPath(token);

    }

}

void _agent_free::onMoveUpdate(_token& token)const {

    if (this->agent->isPickupping()) {

        this->agent->setState(new _agent_occupied(this->agent));

    }

}

void _agent_free::onDraw(const Render& render) const {

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

    textAgentId.draw(render);

    background.draw(render);
    textAgentId.draw(render);

    //    _agent_state::onDraw(render);

    if (this->agent->isDesigned()) {

        sf::Vector2f position(
                this->agent->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
                this->agent->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

        Text textTaskId(
                std::to_string(this->agent->currentTask()->id()),
                position,
                sf::Vector2f(render.GetCell().first / 2, 0),
                sf::Color::Cyan);

        textTaskId.draw(render);

    }

}



