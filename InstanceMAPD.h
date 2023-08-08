/* 
 * File:   InstanceMAPD.h
 * Author: ronaldo
 *
 * Created on 6 de agosto de 2023, 08:54
 */

#ifndef INSTANCEMAPD_H
#define INSTANCEMAPD_H


#include "InstanceMap.h"
#include "InstanceTask.h"


class InstanceMAPD {
public:
    
    friend class System;
    
    InstanceMAPD(InstanceMap* instanceMap, InstanceTask* instanceTask) :
        instanceMap(instanceMap), instanceTask(instanceTask) {}
    
    InstanceMAPD(const InstanceMAPD& other) :
        instanceMap(new InstanceMap(*other.instanceMap)), 
        instanceTask(new InstanceTask(*other.instanceTask)) {
    }

    
    virtual ~InstanceMAPD() {
    
        if(instanceMap != nullptr) delete instanceMap;
        if(instanceTask != nullptr) delete instanceTask;
    
    }
    
    static InstanceMAPD* load(
        std::string map_filename, 
        std::string task_filename){
        
        auto map = InstanceMap::load(map_filename);
        auto tasks = InstanceTask::load(task_filename, [map](unsigned id){
            return *map->getNoBotEndPointById(id);
        });
        
        tasks->getTaskMap().listTasks([map](unsigned step, const Task& task){       
            map->setTaskEndpoint(task.getPickup().GetRow(), task.getPickup().GetColunm());
            map->setTaskEndpoint(task.getDelivery().GetRow(), task.getDelivery().GetColunm());            
            return false;
        });
        
        return new InstanceMAPD(map, tasks);
         
    }
    
    
    
private:
    
    InstanceMap* instanceMap = nullptr;
    InstanceTask* instanceTask = nullptr;

};

#endif /* INSTANCEMAPD_H */

