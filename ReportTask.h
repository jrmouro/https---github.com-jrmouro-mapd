/* 
 * File:   ReportTask.h
 * Author: ronaldo
 *
 * Created on 9 de agosto de 2023, 17:48
 */

#ifndef REPORTTASK_H
#define REPORTTASK_H

#include <map>
#include "_stepPath.h"

class ReportTask : public Task{
public:
    
    enum PathType{
        none,
        pickup,
        delivery,
        rest
    };

    
    
    ReportTask(const Task& task, int taskArrivalStep) :
    Task(task), taskArrivalStep(taskArrivalStep) { }
    
    ReportTask(int id, _site pickup, _site delivery, int taskArrivalStep) :
    Task(id, pickup, delivery), taskArrivalStep(taskArrivalStep) { }
    
    ReportTask(const ReportTask& other) :
            Task(other), 
            taskArrivalStep(other.taskArrivalStep), 
            pickupMap(other.pickupMap),
            deliveryMap(other.deliveryMap),
            restMap(other.restMap){ }

        
    int GetTaskArrivalStep() const {
        return taskArrivalStep;
    }
    
    void SetPath(int agentId, PathType typePath, _stepPath path) {
        
        switch(typePath){
            
            case PathType::pickup:
                pickupMap.insert(std::pair<unsigned, _stepPath>(agentId, path));
                break;
            case PathType::delivery:
                deliveryMap.insert(std::pair<unsigned, _stepPath>(agentId, path));
                break;
            case PathType::rest:
                restMap.insert(std::pair<unsigned, _stepPath>(agentId, path));
                break;
                
        }
        
    }
        
    virtual ~ReportTask(){}
    
    friend std::ostream& operator<<(std::ostream& os, const ReportTask& obj) {
        os << "Task: " << (Task&)obj << std::endl;
        os << "taskArrivalStep: " << obj.taskArrivalStep << std::endl;
        
        os << "pickupPaths:" << std::endl;
        for(auto pair : obj.pickupMap){
            os << "agentId: " << pair.first << std::endl;
            os << pair.second << std::endl;
        }
        
        os << "delivelyPaths:" << std::endl;
        for(auto pair : obj.deliveryMap){
            os << "agentId: " << pair.first << std::endl;
            os << pair.second << std::endl;
        }
        
        os << "restPaths:" << std::endl;
        for(auto pair : obj.restMap){
            os << "agentId: " << pair.first << std::endl;
            os << pair.second << std::endl;
        }
                
        return os;
    }
    
private:
    
    unsigned taskArrivalStep;
    
    std::map<unsigned, _stepPath> pickupMap, deliveryMap, restMap;
    
};

#endif /* REPORTTASK_H */
