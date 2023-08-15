/* 
 * File:   Token.h
 * Author: ronaldo
 *
 * Created on 2 de agosto de 2023, 20:01
 */
#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <map>
#include <iostream>
#include "SiteMap.h"
#include "Task.h"
#include "ReportTask.h"
#include "MapdException.h"
#include "ManhattanAlgorithm.h"
#include "AstarAlgorithm.h"
#include "Agent.h"

class Agent;
class Token : public Drawable{
public:
    
    Token(
        const SiteMap& siteMap, 
        const IntegerMap& integerMap, 
        const std::vector<Agent>& agents,
        const std::vector<_site>& endpoints);
            
    Token(const Token& other) :
        siteMap(other.siteMap), 
        integerMap(other.integerMap),
        agents(other.agents), 
        endpoints(other.endpoints),
        pendingTasks(other.pendingTasks),
        reportTasks(other.reportTasks){ }
    
    virtual ~Token(){
    
        delete _distanceAlgorithm;
        delete _pathAlgorithm;
    
    }  
        
    void addPendingTask(const Task& task){
        this->pendingTasks.insert(std::pair<unsigned, Task>(task.id(), task));
        this->reportTasks.insert(std::pair<unsigned, ReportTask>(task.id(), ReportTask(task, currentStep)));
    }   
      
    void removePendingTask(const Task& task){
        this->pendingTasks.erase(task.id());
    }
    
    void addOpenTask(const Task& task){
        this->openTasks.insert(std::pair<unsigned, Task>(task.id(), task));
    }
    
    void removeOpenTask(const Task& task){
        this->openTasks.erase(task.id());
    }
    
    bool anyPendingTask()const{
        return pendingTasks.empty();
    }
    
    bool anyOpenTask()const{
        return openTasks.empty();
    }
        
    void listAgents(const std::function<bool(Agent&)> function);
    
    void listTasks(const std::function<bool(const Task&)> function)const{
        
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
    
    IntegerMap& getIntegerMap(){
        return integerMap;
    }
    
    void stepping() {
        this->currentStep++;
    }   
    
    const DistanceAlgorithm* distanceAlgorithm()const{
        return _distanceAlgorithm;
    }
    
    const PathAlgorithm* pathAlgorithm()const{
        return _pathAlgorithm;
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
    
    friend std::ostream& operator<<(std::ostream& os, const Token& obj) {
        os << "SiteMap:" << obj.siteMap << std::endl;
        os << "IntegerMap:" << obj.integerMap << std::endl;
        for(auto pair: obj.reportTasks){
            
            os << pair.second << std::endl;
            
        }
        return os;
    }

        
    virtual void draw(const Render& render) const;
            
private:
    
    std::vector<Agent> agents;
    std::map<unsigned, Task> pendingTasks, openTasks;
    std::map<unsigned, ReportTask> reportTasks;
    const SiteMap& siteMap;
    IntegerMap integerMap;
    const std::vector<_site>& endpoints;
    unsigned currentStep = 0;
    
    DistanceAlgorithm* _distanceAlgorithm = new ManhattanAlgorithm();
    PathAlgorithm* _pathAlgorithm = new AstarAlgorithm();
    
    
    
};

#endif /* TOKEN_H */

