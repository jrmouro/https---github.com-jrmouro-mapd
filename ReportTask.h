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

class ReportTask : public _task{
public:
    
    enum PathType{
        task,
        rest,
        c_task
    };

    
    
    ReportTask(const _task& task, int taskArrivalStep) :
    _task(task), taskArrivalStep(taskArrivalStep) { }
    
    ReportTask(int id, _site pickup, _site delivery, int taskArrivalStep) :
    _task(id, pickup, delivery), taskArrivalStep(taskArrivalStep) { }
    
    ReportTask(const ReportTask& other) :
            _task(other), 
            taskArrivalStep(other.taskArrivalStep), 
            taskMap(other.taskMap),
            restMap(other.restMap),
            c_taskMap(other.c_taskMap){ }

        
    int GetTaskArrivalStep() const {
        return taskArrivalStep;
    }
    
    void SetPath(int agentId, PathType typePath, _stepPath path) {
        
        switch(typePath){
            
            case PathType::task:
                taskMap.insert(std::pair<unsigned, _stepPath>(agentId, path));
                break;
            case PathType::rest:
                restMap.insert(std::pair<unsigned, _stepPath>(agentId, path));
                break;
            case PathType::c_task:
                c_taskMap.insert(std::pair<unsigned, _stepPath>(agentId, path));
                break;
                
        }
        
    }
        
    virtual ~ReportTask(){}
    
    friend std::ostream& operator<<(std::ostream& os, const ReportTask& obj) {
        os << "Task: " << (_task&)obj << std::endl;
        os << "taskArrivalStep: " << obj.taskArrivalStep << std::endl;
        
        if(!obj.taskMap.empty()){
            os << "taskPaths:" << std::endl;
            for(auto pair : obj.taskMap){
                os << "agentId: " << pair.first << std::endl;
                os << pair.second << std::endl;
            }
        }
             
        if(!obj.restMap.empty()){
            os << "restPaths:" << std::endl;
            for(auto pair : obj.restMap){
                os << "agentId: " << pair.first << std::endl;
                os << pair.second << std::endl;
            }
        }
        
        if(!obj.c_taskMap.empty()){
            os << "c_taskPaths:" << std::endl;
            for(auto pair : obj.c_taskMap){
                os << "agentId: " << pair.first << std::endl;
                os << pair.second << std::endl;
            }
        }
                
        return os;
    }
    
private:
    
    unsigned taskArrivalStep;
    
    std::map<unsigned, _stepPath> taskMap, restMap, c_taskMap;
    
};

#endif /* REPORTTASK_H */

