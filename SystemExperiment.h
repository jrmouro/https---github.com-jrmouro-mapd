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
        
        std::vector<_agent> _agents;
        
        instanceMAPD->getInstanceMap()->listBotsEndPoints([&_agents](unsigned id, const _site& site){
            
            _agents.push_back(_agent(id, _stepSite(0, site.GetRow(), site.GetColunm())));
            
            return false;
            
        });
        
        _token __token(
            instanceMAPD->getInstanceMap()->getSiteMap(),
            instanceMAPD->getInstanceMap()->getIntegerMap(),
            _agents,
            instanceMAPD->getInstanceMap()->getEndpoints());
        
        
        Render render(
                    std::pair<unsigned, unsigned>(64,64),
                    std::pair<unsigned, unsigned>(
                        instanceMAPD->getInstanceMap()->getColumn_size(),
                        instanceMAPD->getInstanceMap()->getRow_size()),
                    "Test Instance");
        
        Text textStep(
                std::to_string(__token.getCurrentStep()),
                sf::Vector2f(
                    instanceMAPD->getInstanceMap()->getColumn_size()/2 * render.GetCell().first, 
                    0),
                sf::Vector2f(
                    render.GetCell().first, 
                    0),
                sf::Color::White);
        
        render.add(&instanceMAPD->getInstanceMap()->getSiteMap());
        render.add(&__token);
        render.add(&textStep);
        
        _system __system(*instanceMAPD);    
        
        render.loop(800, [&__system, &__token, &textStep](){
            __system.step(__token);
            textStep.setDrawText(std::to_string(__token.getCurrentStep())); 
//            std::cout << __token << std::endl;
        });
        
        std::cout << __token << std::endl;
        
        delete instanceMAPD;
        
    }
    
private:
    
    std::string taskFilename, mapFilename;

};

#endif /* SYSTEMEXPERIMENT_H */

