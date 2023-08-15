/* 
 * File:   InstanceTaskExperiment.h
 * Author: ronaldo
 *
 * Created on 29 de julho de 2023, 08:47
 */

#ifndef INSTANCETASKEXPERIMENT_H
#define INSTANCETASKEXPERIMENT_H

#include "InstanceMapExperiment.h"
#include "InstanceTask.h"

class InstanceTaskExperiment : public InstanceMapExperiment{
    
public:
    
    InstanceTaskExperiment(std::string taskFilename, std::string mapFilename, bool render) :
    InstanceMapExperiment(mapFilename, false), taskFilename(taskFilename), taskRender(render) {
    }
    
    InstanceTaskExperiment(const InstanceTaskExperiment& other) :
    InstanceMapExperiment(other), taskFilename(other.taskFilename), taskRender(taskRender)  { 
    
        if(other.instanceTask != nullptr){
            
            InstanceMap *map = this->instanceMap;
            
            this->instanceTask = InstanceTask::load(other.taskFilename, [map](unsigned id){
                return *map->getNoBotEndPointById(id);
            });
            
        } else {
            
            this->instanceTask = nullptr;
            
        }
    
    }
    
    virtual void run(){
        
        InstanceMapExperiment::run();
        
        InstanceMap *map = this->instanceMap;
        
        std::cout << *map << std::endl;
        
        if(this->instanceTask == nullptr){      
                        
            this->instanceTask = InstanceTask::load(this->taskFilename, [map](unsigned id){
                return *map->getNoBotEndPointById(id);
            });
            
        }
        
        this->instanceTask->getTaskMap().listTasks([map](unsigned step, const Task& task){
       
            map->setTaskEndpoint(task.getPickup().GetRow(), task.getPickup().GetColunm());
            map->setTaskEndpoint(task.getDelivery().GetRow(), task.getDelivery().GetColunm());
            
            return false;
        });
        
        if(this->taskRender){
        
            Render render(
                    std::pair<unsigned, unsigned>(16,16),
                    std::pair<unsigned, unsigned>(this->instanceMap->getColumn_size(),this->instanceMap->getRow_size()),
                    this->filename, this->instanceMap);

            render.loop();
        
        }
        
    }


    virtual ~InstanceTaskExperiment(){
        
        if(this->instanceTask != nullptr){

            delete this->instanceTask;

        }
        
    }
    
protected:
        
    InstanceTask *instanceTask = nullptr;
    std::string taskFilename;        
    
private:
    
    bool taskRender = false;
    
};

#endif /* INSTANCETASKEXPERIMENT_H */

