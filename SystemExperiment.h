/* 
 * File:   SystemExperiment.h
 * Author: ronaldo
 *
 * Created on 8 de agosto de 2023, 14:41
 */

#ifndef SYSTEMEXPERIMENT_H
#define SYSTEMEXPERIMENT_H

#include "Experiment.h"
#include "InstanceMAPD.h"
#include "_system.h"
#include "Render.h"
#include "Text.h"
#include "_agent_free.h"



class SystemExperiment : public Experiment{
public:
    
    SystemExperiment(std::string taskFilename, std::string mapFilename) :
    taskFilename(taskFilename), mapFilename(mapFilename) {}
    
    SystemExperiment(const SystemExperiment& other) :
    taskFilename(other.taskFilename), mapFilename(other.mapFilename) {}


    virtual ~SystemExperiment(){}
    
    virtual void run(){
        
        InstanceMAPD* instanceMAPD = InstanceMAPD::load(mapFilename, taskFilename);
                
        _system system(*instanceMAPD);
        
        Render render(
                    std::pair<unsigned, unsigned>(100,100),
                    std::pair<unsigned, unsigned>(
                        instanceMAPD->getMapColumn_size(),
                        instanceMAPD->getMapRow_size()),
                    "Test Instance");
        
        Text textStep(
                std::to_string(system.getToken().getCurrentStep()),
                sf::Vector2f(
                    instanceMAPD->getMapColumn_size()/2 * render.GetCell().first, 
                    0),
                sf::Vector2f(
                    render.GetCell().first, 
                    0),
                sf::Color::White);
        
        render.add(&instanceMAPD->getMap());
        render.add(&system.getToken());
        render.add(&textStep);
          
        
        render.loop(800, [&system, &textStep](){
            system.step();
            textStep.setDrawText(std::to_string(system.getToken().getCurrentStep())); 
//            std::cout << __token << std::endl;
        });
        
        std::cout << system.getToken() << std::endl;
        
        delete instanceMAPD;
        
    }
    
private:
    
    std::string taskFilename, mapFilename;

};

#endif /* SYSTEMEXPERIMENT_H */

