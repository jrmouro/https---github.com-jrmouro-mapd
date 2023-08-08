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
#include "Task.h"
#include "MapdException.h"
#include "ManhattanAlgorithm.h"
#include "AstarAlgorithm.h"


class Token {
public:
    
    Token(
        const SiteMap& siteMap, 
        const BinaryMap& binaryMap, 
        const std::vector<Agent>& agents,
        const std::vector<_site>& endpoints) : 
            siteMap(siteMap), 
            binaryMap(binaryMap),
            agents(agents),
            endpoints(endpoints){}
    
    Token(
        const SiteMap& siteMap, 
        const BinaryMap& binaryMap, 
        const std::vector<Agent>& agents,
        const std::vector<_site>& endpoints,
        const std::map<Task>& tasks) :
        Token(siteMap, binaryMap, agents, endpoints), pendingTasks(tasks) {}
    
    Token(const Token& other) :
        siteMap(other.siteMap), 
        binaryMap(other.binaryMap),
        agents(other.agents), 
        endpoints(other.endpoints),
        pendingTasks(other.pendingTasks),
        finishedTasks(other.finishedTasks){ }
    
    virtual ~Token(){
    
        delete _distanceAlgorithm;
        delete _pathAlgorithm;
    
    }  
        
    void addPendingTask(const Task& task){
        this->pendingTasks.insert(std::pair<unsigned, Task>(task.id(), task));
    }
    
    void addFinishedTask(const Task& task){
        this->finishedTasks.insert(std::pair<unsigned, Task>(task.id(), task));
    }
    
    void removePendingTask(const Task& task){
        this->pendingTasks.erase(task.id());
    }
    
    
    void listAgents(const std::function<bool(const Agent&)> function){
        
        for (auto agent : agents) {
            
            if(function(agent))return;

        }

    }
    
    void listTasks(const std::function<bool(const Task&)> function){
        
        for (auto taskPair : pendingTasks) {
            
            if(function(taskPair.second))return;

        }

    }
    
    void listEndpoints(const std::function<bool(const _site&)> function){
        
        for (auto endpoint : endpoints) {
            
            if(function(endpoint)) return;

        }
        
    }
        
    const SiteMap& getSiteMap() const {
        return siteMap;
    }
    
    BinaryMap& getBinaryMap() const {
        return binaryMap;
    }
    
    void stepping() {
        this->step++;
    }
    
    bool anyPendingTask()const{
        return pendingTasks.empty();
    }
    
    void finishTask(const Task& task){
        
        auto it = pendingTasks.find(task.id());
        
        if(it != pendingTasks.end()){
            
            pendingTasks.erase(it);
            addFinishedTask(task);
            
        }else{
            
            try {
                std::ostringstream stream;
                stream << "task not found";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
    const DistanceAlgorithm* distanceAlgorithm()const{
        return _distanceAlgorithm;
    }
    
    const PathAlgorithm* pathAlgorithm()const{
        return _pathAlgorithm;
    }
    
    unsigned getCurrentStep() const {
        return step;
    }

    
protected:
    
    void addFinishedTask(const Task& task){
        this->finishedTasks.insert(std::pair<unsigned, Task>(task.id(), task));
    }
            
private:
    
    const std::vector<Agent>& agents;
    std::map<unsigned,Task> pendingTasks, finishedTasks;
    const SiteMap& siteMap;
    BinaryMap binaryMap;
    const std::vector<_site>& endpoints;
    unsigned step = 0;
    
    DistanceAlgorithm* _distanceAlgorithm = new ManhattanAlgorithm();
    PathAlgorithm* _pathAlgorithm = new AstarAlgorithm();
    
};

#endif /* TOKEN_H */

