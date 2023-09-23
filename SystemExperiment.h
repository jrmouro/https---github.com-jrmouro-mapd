/* 
 * File:   SystemExperiment.h
 * Author: ronaldo
 *
 * Created on 8 de agosto de 2023, 14:41
 */

#ifndef SYSTEMEXPERIMENT_H
#define SYSTEMEXPERIMENT_H


#include <ctime>
#include <ratio>
#include <chrono>

#include "Experiment.h"
#include "InstanceMAPD.h"
#include "_system.h"
#include "Render.h"
#include "Text.h"



class SystemExperiment : public Experiment<std::string>{
public:
    
    SystemExperiment(
            const std::string& id,
            const _taskMap& taskMap, 
            _token& token,           
            unsigned cell_size = 0,
            unsigned timestep = 0) :
        Experiment<std::string>(id),   
        cell_size(cell_size),
        timestep(timestep),
        taskMap(taskMap), 
        token(token) { }

    
//    SystemExperiment(const SystemExperiment& other) :
//            Experiment<std::string>(other),
//            cell_size(other.cell_size),
//            timestep(other.timestep),
//            taskMap(other.taskMap), 
//            token(other.token.getInstance()) { }

    virtual ~SystemExperiment(){ }
    
    virtual void run(){
        
        _system system;
        
        if(timestep == 0 || cell_size == 0){
            
            system.run(taskMap, token);
            
        } else {
            
//            token->getStepMap().stepView(token->getCurrentStep());
            
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
            render.loop(timestep, [this, &textStep, &stepping, system](){
                
                textStep.setDrawText(std::to_string(token.getCurrentStep())); 
                
                if(stepping){
                    stepping = system.step(taskMap, token);
                }
                
            });
            
        }                       
                        
    }
    
    

private:    
                    
    const _taskMap& taskMap;
    _token& token;
    
    const unsigned cell_size = 0, timestep = 0; 

};

#endif /* SYSTEMEXPERIMENT_H */

