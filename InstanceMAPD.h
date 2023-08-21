/* 
 * File:   InstanceMAPD.h
 * Author: ronaldo
 *
 * Created on 6 de agosto de 2023, 08:54
 */

#ifndef INSTANCEMAPD_H
#define INSTANCEMAPD_H

#include <vector>
#include "InstanceMap.h"
#include "InstanceTask.h"


class InstanceMAPD {
public:
    
        
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
        
        tasks->getTaskMap().listTasks([map](unsigned step, const _task& task){       
            map->setTaskEndpoint(task.getPickup().GetRow(), task.getPickup().GetColunm());
            map->setTaskEndpoint(task.getDelivery().GetRow(), task.getDelivery().GetColunm());            
            return false;
        });
        
        return new InstanceMAPD(map, tasks);
         
    }
    
    friend std::ostream& operator<<(std::ostream& os, const InstanceMAPD& obj) {
        os << "InstanceMap:" << std::endl;
        os << *obj.instanceMap << std::endl;
        os << "InstanceTask:" << std::endl;
        os << *obj.instanceTask << std::endl;
        return os;
    }

    const _taskMap& getTaskMap() const {
        return instanceTask->getTaskMap();
    }
    
    const _map& getMap() const {
        return instanceMap->getMap();
    }
    
    const _stepMap& getStepMap() const {
        return instanceMap->getStepMap();
    }
        
    void listBotsEndPoints(std::function<bool(unsigned, const _site&)> function) const {
        
        instanceMap->listBotsEndPoints(function);
        
    }
    
    unsigned getLastStepTask() const {
        return instanceTask->getLastStep();
    }
    
    unsigned getMapColumn_size() const {
        return instanceMap->getStepMap().getColumn_size();
    }

    unsigned getMapRow_size() const {
        return instanceMap->getStepMap().getRow_size();
    }
    
    unsigned getMapStep_size() const {
        return instanceMap->getStepMap().getStep_size();
    } 
    
    const std::vector<_site>& getEndpoints() const {
        return instanceMap->getEndpoints();
    }
    
private:
    
    InstanceMap* instanceMap = nullptr;
    InstanceTask* instanceTask = nullptr;

};

#endif /* INSTANCEMAPD_H */

