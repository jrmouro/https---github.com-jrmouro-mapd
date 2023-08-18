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
#include "SiteMap.h"
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
    
    _token(
        const SiteMap& siteMap, 
        const _stepMap& integerMap, 
        const std::vector<_agent>& agents,
        const std::vector<_site>& endpoints);
            
//    _token(const _token& other) :
//        siteMap(other.siteMap), 
//        stepMap(other.stepMap),
//        agents(other.agents), 
//        endpoints(other.endpoints),
//        pendingTasks(other.pendingTasks),
//        reportTasks(other.reportTasks){ }
    
    _token(const _token& other) :
    agents(other.agents), 
    pendingTasks(other.pendingTasks), 
    openTasks(other.openTasks), 
    reportTasks(other.reportTasks), 
    siteMap(other.siteMap), 
    stepMap(other.stepMap), 
    endpoints(other.endpoints), 
    currentStep(other.currentStep), 
    manhattanAlgorithm(other.manhattanAlgorithm), 
    stepAstarAlgorithm(other.stepAstarAlgorithm), 
    endpointsDistanceAlgorithm(other.endpointsDistanceAlgorithm) { }

    
    virtual ~_token(){
    
//        delete manhattanAlgorithm;
//        delete pathAlgorithm;
//        delete endpointsDistanceAlgorithm;
    
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
    
    void listTasks(const std::function<bool(const _task&)> function)const{
        
        for (auto taskPair : pendingTasks) {
            
            if(function(taskPair.second))return;

        }

    }
    
    void listEndpoints(const std::function<bool(const _site&)> function)const{
        
        for (auto endpoint : endpoints) {
            
            if(function(endpoint)) return;

        }
        
    }
        
    const SiteMap& getSiteMap() const {
        return siteMap;
    }
    
    _stepMap& getStepMap(){
        return stepMap;
    }
    
    void stepping() {
        this->currentStep++;
    }   
    
    const _distanceAlgorithm& getManhattanAlgorithm()const{
        return manhattanAlgorithm;
    }
    
    const _distanceAlgorithm& getEndpointsDistanceAlgorithm()const{
        return endpointsDistanceAlgorithm;
    }
    
    const _stepPathAlgorithm& getStepAstarAlgorithm()const{
        return stepAstarAlgorithm;
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
        os << "SiteMap:" << obj.siteMap << std::endl;
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
    const SiteMap& siteMap;
    _stepMap stepMap;
    const std::vector<_site>& endpoints;
    unsigned currentStep = 0;
    
//    _distanceAlgorithm* manhattanAlgorithm = new ManhattanAlgorithm();
//    _distanceAlgorithm* endpointsDistanceAlgorithm = new _endpointsDistanceAlgorithm();
//    _stepPathAlgorithm* pathAlgorithm = new _stepAstarAlgorithm();
    
    ManhattanAlgorithm manhattanAlgorithm;
    _stepAstarAlgorithm stepAstarAlgorithm;
    _endpointsDistanceAlgorithm endpointsDistanceAlgorithm;
    
};

#endif /* _TOKEN_H */

