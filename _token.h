/* 
 * File:   _token.h
 * Author: ronaldo
 *
 * Created on 15 de agosto de 2023, 00:01
 */
#ifndef _TOKEN_H
#define _TOKEN_H

#include <vector>
#include <map>
#include <iostream>
#include "_map.h"
#include "_task.h"
#include "ReportTask.h"
#include "MapdException.h"
#include "ManhattanAlgorithm.h"
#include "_endPointsDistanceAlgorithm.h"
#include "_stepAstarAlgorithm.h"
#include "_agent.h"

class _agent;
class _token : public Drawable{
public:
    
    _token(){}            
    
    _token(const _token& other) :
    agents(other.agents), 
    pendingTasks(other.pendingTasks), 
    openTasks(other.openTasks), 
    reportTasks(other.reportTasks), 
    currentStep(other.currentStep) { }

    
    virtual ~_token(){ }  
    
    void addAgent(const _agent& agent){
        this->agents.push_back(agent);
    }
        
    void addPendingTask(const _task& task){
        this->pendingTasks.insert(std::pair<unsigned, _task>(task.id(), task));
        this->reportTasks.insert(std::pair<unsigned, ReportTask>(task.id(), ReportTask(task, currentStep)));
    }   
      
    void removePendingTask(const _task& task){
        this->pendingTasks.erase(task.id());
    }
    
    void addOpenTask(const _task& task){
        this->openTasks.insert(std::pair<unsigned, _task>(task.id(), task));
    }
    
    void removeOpenTask(const _task& task){
        this->openTasks.erase(task.id());
    }
    
    bool anyPendingTask()const{
        return pendingTasks.empty();
    }
    
    bool anyOpenTask()const{
        return openTasks.empty();
    }
        
    void listAgents(const std::function<bool(_agent&)> function);
    
    void listPendingTasks(const std::function<bool(const _task&)> function)const{
        
        for (auto taskPair : pendingTasks) {
            
            if(function(taskPair.second))return;

        }

    }
    
//    void listEndpoints(const std::function<bool(const _site&)> function)const{
//        
//        for (auto endpoint : endpoints) {
//            
//            if(function(endpoint)) return;
//
//        }
//        
//    }
            
    void stepping() {
        this->currentStep++;
    }   
        
    unsigned getCurrentStep() const {
        return currentStep;
    }
    
    void reportTaskUpdate(unsigned taskId, unsigned agentId, ReportTask::PathType pathType, const _stepPath& path){
        
        std::map<unsigned, ReportTask>::iterator it = reportTasks.find(taskId);
        
        if(it != reportTasks.end()){
            
            it->second.SetPath(agentId, pathType, path);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "report task not found";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }  
    
    friend std::ostream& operator<<(std::ostream& os, const _token& obj) {
        os << "current step: " << obj.currentStep << std::endl;
        for(auto pair: obj.reportTasks){
            
            os << pair.second << std::endl;
            
        }
        return os;
    }

    virtual void draw(const Render& render) const;
            
private:
    
    std::vector<_agent> agents;
    std::map<unsigned, _task> pendingTasks, openTasks;
    std::map<unsigned, ReportTask> reportTasks;
    unsigned currentStep = 0;
    
};

#endif /* _TOKEN_H */

