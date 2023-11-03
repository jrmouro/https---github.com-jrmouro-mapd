/* 
 * File:   GA_SystemExperiment.cpp
 * Author: ronaldo
 * 
 * Created on 12 de outubro de 2023, 09:24
 */

#include "GA_SystemExperiment.h"
#include "Render.h"
#include "Text.h"

GA_SystemExperiment::GA_SystemExperiment(
        const std::string id, 
        _agentsTasksAllocator& agentsTasksAllocator,
        _ga_token& token, 
        const _taskMap& taskMap,
        unsigned cell_size, 
        unsigned timestep) :
    Experiment<std::string>(id), 
    system(agentsTasksAllocator), 
    token(token), 
    taskMap(taskMap),
    cell_size(cell_size),
    timestep(timestep){ }

GA_SystemExperiment::GA_SystemExperiment(const GA_SystemExperiment& other) :
            Experiment<std::string>(other),
            system(other.system), 
            token(other.token), 
            taskMap(other.taskMap), 
            cell_size(other.cell_size), 
            timestep(other.timestep) {}

    
    
GA_SystemExperiment::~GA_SystemExperiment(){}

void GA_SystemExperiment::run(){

    if(timestep == 0 || cell_size == 0){

        system.run(taskMap, token);

    } else {

        Render render(
                std::pair<unsigned, unsigned>(cell_size,cell_size),
                std::pair<unsigned, unsigned>(
                    token.getMap().getColumn_size(),
                    token.getMap().getRow_size()),
                this->id());

        Text textStep(
                std::to_string(token.getCurrentStep()),
                sf::Vector2f(
                    token.getMap().getColumn_size()/2 * render.GetCell().first, 
                    0),
                sf::Vector2f(
                    render.GetCell().first, 
                    0),
                sf::Color::White);

        render.add(&token.getMap());
        render.add(&token);
        render.add(&textStep);

        bool stepping = true;
        render.loop(timestep, [this, &textStep, &stepping](){

            textStep.setDrawText(std::to_string(token.getCurrentStep())); 

            if(stepping){
                stepping = system.step(taskMap, token);
            }

        });

    }                       

}

