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



class SystemExperiment : public Experiment, public Writable{
public:
    
    SystemExperiment(
            std::string taskFilename, 
            std::string mapFilename, 
            _system::TokenType tokenType, 
            float taskThreshold, 
            float carryThreshold, 
            int currentEnergyLevelAgent, 
            int maximumEnergyLevelAgent, 
            int chargedEnergyLevelAgent,
            int criticalEnergyLevelAgent,
            unsigned cell_size = 0, 
            unsigned timestep = 0) :
        cell_size(cell_size),
        timestep(timestep),
        taskFilename(taskFilename), 
        mapFilename(mapFilename), 
        tokenType(tokenType), 
        taskThreshold(taskThreshold), 
        carryThreshold(carryThreshold), 
        maximumEnergyLevelAgent(maximumEnergyLevelAgent), 
        chargedEnergyLevelAgent(chargedEnergyLevelAgent), 
        currentEnergyLevelAgent(currentEnergyLevelAgent), 
        criticalEnergyLevelAgent(criticalEnergyLevelAgent) {
    }

    
    SystemExperiment(const SystemExperiment& other) :
            cell_size(other.cell_size),
            timestep(other.timestep),
            taskFilename(other.taskFilename), 
            mapFilename(other.mapFilename), 
            tokenType(other.tokenType), 
            taskThreshold(other.taskThreshold), 
            carryThreshold(other.carryThreshold), 
            maximumEnergyLevelAgent(other.maximumEnergyLevelAgent),
            chargedEnergyLevelAgent(other.chargedEnergyLevelAgent),
            currentEnergyLevelAgent(other.currentEnergyLevelAgent), 
            criticalEnergyLevelAgent(other.criticalEnergyLevelAgent) {
    }

    virtual ~SystemExperiment(){
    
        if(system != nullptr){
            delete system;
        }
    }
    
    virtual void writeHeader(std::ostream& fs) const {
        
        if(system != nullptr){
            
            system->writeHeader(fs);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "uninitialized system";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }  
    
    virtual void writeRow(std::ostream& fs) const {
        
        if(system != nullptr){
            
            system->writeRow(fs);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "uninitialized system";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }      
        
    }
    
    virtual void run(){
        
        if(system != nullptr){
            delete system;
        }
        
        system = new _system(
                tokenType, 
                taskFilename,
                mapFilename,                
                taskThreshold, 
                carryThreshold, 
                currentEnergyLevelAgent, 
                maximumEnergyLevelAgent, 
                chargedEnergyLevelAgent, 
                criticalEnergyLevelAgent);
        
        if(timestep == 0 || cell_size == 0){
            
            system->run();
            
        } else {
            
            Render render(
                    std::pair<unsigned, unsigned>(cell_size,cell_size),
                    std::pair<unsigned, unsigned>(
                        system->getInstanceMAPD().getMapColumn_size(),
                        system->getInstanceMAPD().getMapRow_size()),
                    "Test Instance");
        
            Text textStep(
                    std::to_string(system->getToken().getCurrentStep()),
                    sf::Vector2f(
                        system->getInstanceMAPD().getMapColumn_size()/2 * render.GetCell().first, 
                        0),
                    sf::Vector2f(
                        render.GetCell().first, 
                        0),
                    sf::Color::White);

            render.add(&system->getInstanceMAPD().getMap());
            render.add(&system->getToken());
            render.add(&textStep);

            bool stepping = true;
            render.loop(timestep, [this, &textStep, &stepping](){
                
                if(stepping){
                    stepping = this->system->step();
                }
                
                textStep.setDrawText(std::to_string(this->system->getToken().getCurrentStep())); 
                
            });
            
        }
                
        std::cout << "token: " << system->getToken() << std::endl;
        std::cout << "steps: " << system->getToken().getCurrentStep() << std::endl;
        std::cout << "energy: " << system->getToken().energy() << std::endl;
        
                        
    }
    
    
private:
    
    _system* system = nullptr;
        
    std::string taskFilename, 
            mapFilename;
    
    _system::TokenType tokenType;
    
    float taskThreshold, 
            carryThreshold;
    
    const unsigned cell_size = 0, timestep = 0;
    
    const int maximumEnergyLevelAgent, 
            chargedEnergyLevelAgent, 
            currentEnergyLevelAgent, 
            criticalEnergyLevelAgent;

};

#endif /* SYSTEMEXPERIMENT_H */

