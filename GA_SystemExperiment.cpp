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
        const _ga_system& system, 
        const _ga_token& token, 
        const _taskMap& taskMap,
        unsigned cell_size, 
        unsigned timestep) :
    Experiment<std::string>(id), 
    system(system), 
    token(token), 
    taskMap(taskMap),
    cell_size(cell_size),
    timestep(timestep){ }

void GA_SystemExperiment::run(){

    if(timestep == 0 || cell_size == 0){

        system.run(taskMap, token);

    } else {

        Render render(
                std::pair<unsigned, unsigned>(cell_size,cell_size),
                std::pair<unsigned, unsigned>(
                    system.getMap().getColumn_size(),
                    system.getMap().getRow_size()),
                this->id());

        Text textStep(
                std::to_string(token.getStep()),
                sf::Vector2f(
                    system.getMap().getColumn_size()/2 * render.GetCell().first, 
                    0),
                sf::Vector2f(
                    render.GetCell().first, 
                    0),
                sf::Color::White);

        render.add(&system.getMap());
        render.add(&token);
        render.add(&textStep);

        bool stepping = true;
        render.loop(timestep, [this, &textStep, &stepping](){

            textStep.setDrawText(std::to_string(token.getStep())); 

            if(stepping){
                stepping = system.step(taskMap, token);
            }

        });

    }                       

}

