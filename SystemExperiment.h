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



class SystemExperiment : public Experiment{
public:
    
    SystemExperiment(
            std::string taskFilename, 
            std::string mapFilename, 
            _system::TokenType tokenType, 
            float taskThreshold, 
            float carryThreshold, 
            int currentEnergyLevelAgent, 
            int maximumEnergyLevelAgent, 
            int chargingEnergyLevelAgent,
            int criticalEnergyLevelAgent) :
    taskFilename(taskFilename), 
            mapFilename(mapFilename), 
            tokenType(tokenType), 
            taskThreshold(taskThreshold), 
            carryThreshold(carryThreshold), 
            maximumEnergyLevelAgent(maximumEnergyLevelAgent), 
            chargingEnergyLevelAgent(chargingEnergyLevelAgent), 
            currentEnergyLevelAgent(currentEnergyLevelAgent), 
            criticalEnergyLevelAgent(criticalEnergyLevelAgent) {
    }

    
    SystemExperiment(const SystemExperiment& other) :
            taskFilename(other.taskFilename), 
            mapFilename(other.mapFilename), 
            tokenType(other.tokenType), 
            taskThreshold(other.taskThreshold), 
            carryThreshold(other.carryThreshold), 
            maximumEnergyLevelAgent(other.maximumEnergyLevelAgent),
            chargingEnergyLevelAgent(other.chargingEnergyLevelAgent),
            currentEnergyLevelAgent(other.currentEnergyLevelAgent), 
            criticalEnergyLevelAgent(other.criticalEnergyLevelAgent) {
    }



    virtual ~SystemExperiment(){}
    
    virtual void run(){
        
        InstanceMAPD* instanceMAPD = InstanceMAPD::load(mapFilename, taskFilename);
                
        _system system(
                tokenType, 
                *instanceMAPD, 
                taskThreshold, 
                carryThreshold, 
                currentEnergyLevelAgent, 
                maximumEnergyLevelAgent, 
                chargingEnergyLevelAgent, 
                criticalEnergyLevelAgent);
        
        Render render(
                    std::pair<unsigned, unsigned>(16,16),
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
          
        
        render.loop(10, [&system, &textStep](){
            system.step();
            textStep.setDrawText(std::to_string(system.getToken().getCurrentStep())); 
        });
        std::cout << "steps: " << system.getToken() << std::endl;
        std::cout << "steps: " << system.getToken().getCurrentStep() << std::endl;
        std::cout << "energy: " << system.getToken().energy() << std::endl;
        
        delete instanceMAPD;
        
    }
    
private:
    
    std::string taskFilename, 
            mapFilename;
    
    _system::TokenType tokenType;
    
    float taskThreshold, 
            carryThreshold;
    const int maximumEnergyLevelAgent, 
            chargingEnergyLevelAgent, 
            currentEnergyLevelAgent, 
            criticalEnergyLevelAgent;

};

#endif /* SYSTEMEXPERIMENT_H */

