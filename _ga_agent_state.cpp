/* 
 * File:   _ga_agent_state.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 07:34
 */

#include "_ga_agent_state.h"
#include "_ga_agent.h"

#include "Circle.h"
#include "Text.h"
#include "Render.h"

void _ga_agent_state::changeState(_ga_agent& agent, _ga_agent_state* state) const{
    agent.change_state(state);
}

void _ga_agent_state::onDraw(const Render& render, const _ga_agent& agent) const {

    sf::Color energy = sf::Color::Green;
    if (agent.isAtEnergyChargedLevel()) {
        energy = sf::Color::Blue;
    } else if (agent.isAtEnergyCriticalLevel()) {
        energy = sf::Color::Yellow;
    } else if (agent.isAtEnergyDeadLevel()) {
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