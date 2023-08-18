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

void _agent_free::onUpdatePath(_token& token,  _agent* agent) const {
    
    if (agent->isParked()) { // request token condition
        
        if (!agent->isResting()){ //retirar depois
            
            try {
                std::ostringstream stream;
                stream << "no resting";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }                                  
            
        } 
        
        _task newTask;
        
        if (agent->selectNewTask(token, newTask)) {
            
            agent->designTask(newTask);

            agent->updateTaskPath(token, newTask);
            
            if (agent->isParked()) {
                
                token.removeOpenTask(newTask);

                agent->undesignTask();
                
                agent->updateTrivialPath(token); 
                
            } else if (agent->isPickupping()) { // caso do agente já se encontrar em pickup site

                agent->changeState(new _agent_occupied());
                
                return;

            }
            
            std::cout << "aqui" << std::endl;

            return;

        }
        
        _task conflit;

//        if (agent->isConflictingRestEndpoint(token, &conflit)) {
//
//            agent->updateRestEndpointPath(token, conflit);
//
//            return;
//
//        }
        
        agent->updateTrivialPath(token);       
        
    }

}

void _agent_free::onMoveUpdate(_token& token,  _agent* agent)const {

    if (agent->isPickupping()) {

        agent->changeState(new _agent_occupied());

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

    //    _agent_state::onDraw(render);

    if (agent->isDesigned()) {

        sf::Vector2f position(
                agent->currentSite().GetColunm() * render.GetCell().first + render.GetCell().first / 2,
                agent->currentSite().GetRow() * render.GetCell().second + render.GetCell().first / 2);

        Text textTaskId(
                std::to_string(agent->getCurrentTask().id()),
                position,
                sf::Vector2f(render.GetCell().first / 2, 0),
                sf::Color::Cyan);

        textTaskId.draw(render);

    }

}



