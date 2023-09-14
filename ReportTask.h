/* 
 * File:   ReportTask.h
 * Author: ronaldo
 *
 * Created on 9 de agosto de 2023, 17:48
 */

#ifndef REPORTTASK_H
#define REPORTTASK_H

#include <map>
#include "_agent.h"
#include "_stepPath.h"

class ReportTask : public _task{
public:
    
    enum PathType{
        task,
        rest,
        backward_task,
        charging
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
            c_taskMap(other.c_taskMap),
            chargingMap(other.c_taskMap){ }

        
    int GetTaskArrivalStep() const {
        return taskArrivalStep;
    }
    
    void SetPath(const _agent& agent, PathType typePath, const _stepPath& path) {
        
        switch(typePath){
            
            case PathType::task:
                taskMap.insert(std::pair<unsigned, _stepPath>(agent.id(), path));
                break;
            case PathType::rest:
                restMap.insert(std::pair<unsigned, _stepPath>(agent.id(), path));
                break;
            case PathType::backward_task:
                c_taskMap.insert(std::pair<unsigned, _stepPath>(agent.id(), path));
                break;
            case PathType::charging:
                chargingMap.insert(std::pair<unsigned, _stepPath>(agent.id(), path));
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
        
        if(!obj.chargingMap.empty()){
            os << "chargingPaths:" << std::endl;
            for(auto pair : obj.chargingMap){
                os << "agentId: " << pair.first << std::endl;
                os << pair.second << std::endl;
            }
        }
                
        return os;
    }
    
    void listStepSiteByAgent(const _agent& agent, const std::function<bool(int, PathType, _stepSite)>& function)const{
        
        listStepSite([agent, function](int agentId, PathType pathType, const _stepSite& site){
            
            if(agent.id() == agentId) return function(agentId, pathType, site);
            
            return false;
            
        });
        
    }
    
    void listStepSite(const std::function<bool(int, PathType, _stepSite)>& function)const{
        
        bool flag = false;
        for(auto pair : taskMap){
            
            pair.second.backward([pair, function, &flag](const _stepSite& site){
                
                flag = function(pair.first, PathType::task, site);
                
                return flag;
                
            });
            
            if(flag)return;
        }
        
        if(!flag){
        
            for(auto pair : restMap){
                
                pair.second.backward([pair, function, &flag](const _stepSite& site){
                
                    flag = function(pair.first, PathType::rest, site);

                    return flag;

                });


                if(flag)return;
            }
            if(!flag){
            
                for(auto pair : c_taskMap){
                    
                    pair.second.backward([pair, function, &flag](const _stepSite& site){
                
                        flag = function(pair.first, PathType::backward_task, site);

                        return flag;

                    });

                    if(flag)return;
                    
                }
                
                if(!flag){
                    
                    for(auto pair : chargingMap){
                    
                        pair.second.backward([pair, function, &flag](const _stepSite& site){

                            flag = function(pair.first, PathType::charging, site);

                            return flag;

                        });

                        if(flag)return;

                    }
                    
                }
                
            }
        
        }
        
    }
        
private:
    
    unsigned taskArrivalStep;
    
    std::map<unsigned, _stepPath> taskMap, restMap, c_taskMap, chargingMap;
    
};

#endif /* REPORTTASK_H */

