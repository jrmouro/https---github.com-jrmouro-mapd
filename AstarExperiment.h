/* 
 * File:   AstarExperiment.h
 * Author: ronaldo
 *
 * Created on 29 de julho de 2023, 09:22
 */

#ifndef ASTAREXPERIMENT_H
#define ASTAREXPERIMENT_H

#include "InstanceTaskExperiment.h"
#include "AstarAlgorithm.h"

class AstarExperiment : public InstanceTaskExperiment{
public:
    
    AstarExperiment(std::string taskFilename, std::string mapFilename, bool render) :
    InstanceTaskExperiment(taskFilename, mapFilename, render) {}
    
    AstarExperiment(const AstarExperiment& other) :
    InstanceTaskExperiment(other) {}
    
    virtual void run(){
                
        InstanceTaskExperiment::run();
        
        AstarAlgorithm astar;
        
        auto siteMap = this->instanceMap->getSiteMap();
        auto task = this->instanceTask->getTaskMap()->getTaskbyId(0);
        
        Path path;
        
        astar.solve(*siteMap, task.getPickup(), task.getDelivery(), path);
        
        
    }


    
    virtual ~AstarExperiment(){
        
    }
    
private:

};

#endif /* ASTAREXPERIMENT_H */

