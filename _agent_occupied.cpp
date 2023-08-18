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

void _agent_occupied::onUpdatePath(_token& token,  _agent* agent)const {

    if (agent->isParked()) { // request token condition
        
        if (agent->isResting()){ //retirar depois
            
            try {
                std::ostringstream stream;
                stream << "no resting";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }                                  
            
        } 
        
        if (!agent->isDelivering()){ //retirar depois
            
            try {
                std::ostringstream stream;
                stream << "no delivering";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }                                  
            
        } 
        
        

        token.removeOpenTask(agent->getCurrentTask());

        agent->undesignTask();
        
        _task newTask;
                        
        if (agent->selectNewTask(token, newTask)) {

            agent->updateTaskPath(token, newTask);
            
            if (agent->isParked()) {
                
                token.removeOpenTask(agent->getCurrentTask());

                agent->undesignTask();
                
                agent->updateTrivialPath(token); 
                
                agent->changeState(new _agent_free());
                
            } else if (!agent->isPickupping()) { // caso do agente já se encontrar em pickup site

                agent->changeState(new _agent_free());
                
                return;

            } 

            return;

        } else {
        
            _task conflit;

//            if (agent->isConflictingRestEndpoint(token, &conflit)) {
//
//                agent->updateRestEndpointPath(token, conflit);
//
//            } else {
//
//                agent->updateTrivialPath(token);  
//
//            }

            agent->changeState(new _agent_free());
        
        }
        
    }

}


void _agent_occupied::onMoveUpdate(_token& token,  _agent* agent)const {

//   if (!this->agent->isDelivering()){
//       
//       this->agent->setState(new _agent_free(this->agent));
//       
//   }

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
