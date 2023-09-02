/* 
 * File:   MultiSystemExperiment.h
 * Author: ronaldo
 *
 * Created on 31 de agosto de 2023, 04:45
 */

#ifndef MULTISYSTEMEXPERIMENT_H
#define MULTISYSTEMEXPERIMENT_H

#include "SystemExperiment.h"


class MultiSystemExperiment : public Experiment{
    
public:
    
    MultiSystemExperiment(
            std::string resultFilename, 
            std::vector<std::string> taskFilenames,             
            std::vector<std::string> mapFilenames,
            std::vector<_system::TokenType> tokenTypes, 
            float taskThreshold, 
            float carryThreshold,             
            const int currentEnergyLevelAgent,
            const int maximumEnergyLevelAgent, 
            const int chargedEnergyLevelAgent,  
            const int criticalEnergyLevelAgent, 
            const unsigned cell_size = 0, 
            const unsigned timestep = 0) :
                    resultFilename(resultFilename),
                    mapFilenames(mapFilenames), 
                    taskFilenames(taskFilenames), 
                    tokenTypes(tokenTypes), 
                    taskThreshold(taskThreshold), 
                    carryThreshold(carryThreshold), 
                    maximumEnergyLevelAgent(maximumEnergyLevelAgent), 
                    chargedEnergyLevelAgent(chargedEnergyLevelAgent), 
                    currentEnergyLevelAgent(currentEnergyLevelAgent), 
                    criticalEnergyLevelAgent(criticalEnergyLevelAgent), 
                    cell_size(cell_size), 
                    timestep(timestep) {}

    MultiSystemExperiment(const MultiSystemExperiment& other) :
            resultFilename(other.resultFilename),
            mapFilenames(other.mapFilenames), 
            taskFilenames(other.taskFilenames), 
            tokenTypes(other.tokenTypes), 
            taskThreshold(other.taskThreshold), 
            carryThreshold(other.carryThreshold), 
            maximumEnergyLevelAgent(other.maximumEnergyLevelAgent), 
            chargedEnergyLevelAgent(other.chargedEnergyLevelAgent), 
            currentEnergyLevelAgent(other.currentEnergyLevelAgent), 
            criticalEnergyLevelAgent(other.criticalEnergyLevelAgent), 
            cell_size(other.cell_size), 
            timestep(other.timestep) { }

    
    virtual void run(){
        
        std::ofstream ofs (resultFilename, std::ofstream::out);
        
        bool headerFlag = true;
        
        for (auto tokenType : tokenTypes) {
            
            for (auto mapFilename : mapFilenames) {
                
                for (auto taskFilename : taskFilenames) {
                    
                    SystemExperiment experiment(
                            taskFilename,
                            mapFilename,
                            tokenType,
                            taskThreshold, 
                            carryThreshold, 
                            currentEnergyLevelAgent, 
                            maximumEnergyLevelAgent, 
                            chargedEnergyLevelAgent,
                            criticalEnergyLevelAgent,
                            cell_size, 
                            timestep);  
                    
                    experiment.run();
                    
                    if(headerFlag){
                        headerFlag = false;
                        experiment.writeHeader(ofs);
                        Writable::endlWrite(experiment, ofs);                      
                    }
                    
                    experiment.writeRow(ofs);
                    Writable::endlWrite(experiment, ofs);

                }

            }

        }
         
        ofs.close();        
        
    }
    
    virtual ~MultiSystemExperiment(){}
    
private:
    
    std::string resultFilename;
    
    std::vector<std::string> mapFilenames, taskFilenames;
    std::vector<_system::TokenType> tokenTypes;
    
    float taskThreshold, 
            carryThreshold;
        
    const int maximumEnergyLevelAgent, 
            chargedEnergyLevelAgent, 
            currentEnergyLevelAgent, 
            criticalEnergyLevelAgent;
    
    const unsigned cell_size = 0, timestep = 0;

};

#endif /* MULTISYSTEMEXPERIMENT_H */

