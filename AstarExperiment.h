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
    
    AstarExperiment(std::string taskFilename, std::string mapFilename, bool render, unsigned taskId = 0) :
    InstanceTaskExperiment(taskFilename, mapFilename, false), astarRender(render), taskId(taskId){}
    
    AstarExperiment(const AstarExperiment& other) :
    InstanceTaskExperiment(other), astarRender(other.astarRender), taskId(other.taskId) {}
    
    virtual void run(){
                
        InstanceTaskExperiment::run();
        
        AstarAlgorithm astar;
        
        auto siteMap = this->instanceMap->getSiteMap();
        auto task = this->instanceTask->getTaskMap()->getTaskbyId(this->taskId);
        
        Path path;
        
        astar.solve(*siteMap, task.getPickup(), task.getDelivery(), path);
        
        if(this->astarRender){
        
            Render render(
                    std::pair<unsigned, unsigned>(16,16),
                    std::pair<unsigned, unsigned>(this->instanceMap->getColumn_size(),this->instanceMap->getRow_size()),
                    this->filename, this->instanceMap);
            
            render.add(&path);

            render.loop();
        
        }
        
        
    }
    
    virtual ~AstarExperiment(){
        
    }
    
private:
    
    bool astarRender = false;
    unsigned taskId = 0;

};

#endif /* ASTAREXPERIMENT_H */

