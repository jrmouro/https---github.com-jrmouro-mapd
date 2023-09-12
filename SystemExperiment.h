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



class SystemExperiment : public Experiment, public Writable{
public:
    
    SystemExperiment(
            std::string taskFilename, 
            std::string mapFilename, 
            _system::TokenType tokenType,              
            int currentEnergyLevelAgent, 
            int maximumEnergyLevelAgent, 
            int chargedEnergyLevelAgent,
            int criticalEnergyLevelAgent,
            unsigned cell_size = 0, 
            unsigned timestep = 0,
            float pickup_threshold = .0f, 
            float delivery_threshold = .0f) :
        cell_size(cell_size),
        timestep(timestep),
        taskFilename(taskFilename), 
        mapFilename(mapFilename), 
        tokenType(tokenType), 
        pickup_threshold(pickup_threshold), 
        delivery_threshold(delivery_threshold), 
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
            pickup_threshold(other.pickup_threshold), 
            delivery_threshold(other.delivery_threshold), 
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
                currentEnergyLevelAgent, 
                maximumEnergyLevelAgent, 
                chargedEnergyLevelAgent, 
                criticalEnergyLevelAgent,
                pickup_threshold, 
                delivery_threshold);
        
        
        
        std::cout << "Experiment: " << std::endl;
        std::cout << " - mapFilename: " << mapFilename << std::endl;
        std::cout << " - taskFilename: " << taskFilename << std::endl;
        std::cout << " - token: " << system->getToken().id() << std::endl;
        
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        
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
        
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        
        std::cout << " - makespan: " << system->getToken().getCurrentStep() << std::endl;
        std::cout << " - energy expenditure: " << system->getToken().energyExpenditure() << std::endl;
        std::cout << " - peding tasks: " << system->getToken().getPendingTaskAmount() << std::endl;
        std::cout << " - finished tasks: " << system->getToken().getFinishedTaskAmount() << std::endl;
        std::cout << " - duration: " << time_span.count() << " seconds." << std::endl << std::endl;                        
                        
    }
    
    
private:
    
    _system* system = nullptr;
        
    std::string taskFilename, 
            mapFilename;
    
    _system::TokenType tokenType;
    
    float pickup_threshold, 
            delivery_threshold;
    
    const unsigned cell_size = 0, timestep = 0;
    
    const int maximumEnergyLevelAgent, 
            chargedEnergyLevelAgent, 
            currentEnergyLevelAgent, 
            criticalEnergyLevelAgent;

};

#endif /* SYSTEMEXPERIMENT_H */

