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
#include <unordered_set>
#include <iostream>
#include "Writable.h"
#include "_map.h"
#include "_c_task.h"
#include "ReportTask.h"
#include "MapdException.h"
#include "ManhattanAlgorithm.h"
#include "_endPointsDistanceAlgorithm.h"
#include "_stepAstarAlgorithm.h"
#include "_agent.h"
#include "ReportTaskMap.h"

class _agent;
class _updateTokenAlgorithms;
class _token : public Identifiable<std::string>, public Drawable, public Writable{
public:
    
    enum TokenUpdateType{
        task,
        charging_task,
        rest,
        charging_rest,
        trivial,
        charging_trivial,
        none
    };
    
    _token(
            const _map& map, 
            _stepMap& stepMap, 
            std::vector<_site>& endpoints, 
            std::vector<_site>& chargingEndpoints, 
            const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm):
    map(map),
    stepMap(stepMap),
    endpoints(endpoints),
    nonTaskDeliveryEndpoints(),
    taskDeliveryEndpoints(),
    chargingEndpoints(chargingEndpoints),
    endpointsDistanceAlgorithm(endpointsDistanceAlgorithm){
    
        for (auto endpoint : endpoints) {

            nonTaskDeliveryEndpoints.insert(std::pair<unsigned, _site>(
                endpoint.linearIndex(map.getColumn_size()), 
                endpoint));
            
        }

    
    }            
    
    _token(const _token& other) :
    map(other.map),
    stepMap(other.stepMap),
    endpoints(other.endpoints),
    nonTaskDeliveryEndpoints(other.nonTaskDeliveryEndpoints),
    taskDeliveryEndpoints(other.taskDeliveryEndpoints),
    chargingEndpoints(other.chargingEndpoints),
    endpointsDistanceAlgorithm(other.endpointsDistanceAlgorithm),
    assignTaskAgent(other.assignTaskAgent),
    reportTaskMap(other.reportTaskMap), 
    currentStep(other.currentStep) { 
        
        for (auto p_c_task : backwardTasks) {
            
            std::vector<_task*> vtask;
            
            for (auto elem : p_c_task.second) {
                
                vtask.push_back(elem->instance());

            }

            backwardTasks.insert(std::pair<unsigned, std::vector<_task*>>(p_c_task.first, vtask));
            
        }

        
        for (auto pagent : other.agents) {
            
            agents.insert(std::pair<unsigned, _agent*>(pagent.second->id(), pagent.second->instance()));

        }
    
        for (auto ptask : other.pendingTasks) {
            
            pendingTasks.insert(std::pair<unsigned, _task*>(ptask.second->id(), ptask.second->instance()));

        }
        
        for (auto ptask : other.assignedTasks) {
            
            assignedTasks.insert(std::pair<unsigned, _task*>(ptask.second->id(), ptask.second->instance()));

        }
        
        for (auto ptask : other.runningTasks) {
            
            runningTasks.insert(std::pair<unsigned, _task*>(ptask.second->id(), ptask.second->instance()));

        }
        
        for (auto ptask : other.finishedTasks) {
            
            finishedTasks.insert(std::pair<unsigned, _task*>(ptask.second->id(), ptask.second->instance()));

        }

    
    }
    
    virtual _token* getInstance() const = 0;
    
    virtual ~_token(){ 
        
        for (auto p_c_task : backwardTasks) {
                        
            for (auto task : p_c_task.second) {
                
                delete task;

            }
            
        }
        
        for (auto pagent : agents) delete pagent.second;
    
        for (auto ptask : pendingTasks) delete ptask.second;
        
        for (auto ptask : assignedTasks) delete ptask.second;
        
        for (auto ptask : runningTasks) delete ptask.second;
        
        for (auto ptask : finishedTasks) delete ptask.second;
    
    } 
    
    virtual void writeHeader(std::ostream& fs) const {
        Writable::strWrite(*this, fs, "token_id", true); 
        Writable::strWrite(*this, fs, "current_step", true);
        Writable::strWrite(*this, fs, "energy", true);
        Writable::strWrite(*this, fs, "finished_tasks", true);
    }   
    
    virtual void writeRow(std::ostream& fs) const {
        Writable::strWrite(*this, fs, id(), true); 
        Writable::uintWrite(*this, fs, currentStep, true);
        Writable::intWrite(*this, fs, energyExpenditure(), true);
        Writable::uintWrite(*this, fs, finishedTasks.size());
    }
        
    virtual std::string id() const {
        
        return "tp";
        
    }
    
    bool isChargingSite(const _agent& agent)const{
        
        return isChargingSite(agent.currentSite());
        
    }
    
    bool isChargingSite(const _site& site)const{
        
        return map.getType(site) == _map::Type::bot;
        
    }
        
    void assignTask(const _task& task, const _agent& agent){
        
        std::map<unsigned, _task*>::const_iterator it = pendingTasks.find(task.id());
        
        if(it != pendingTasks.end()){
            
            assignTaskAgent.insert(std::pair<unsigned, unsigned>(task.id(), agent.id()));
            assignedTasks.insert(std::pair<unsigned, _task*>(task.id(), it->second));
            
            pendingTasks.erase(it);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "pending task not found: " << task;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
    void runTask(const _task& task){
        
        std::map<unsigned, _task*>::const_iterator it = assignedTasks.find(task.id());
        
        if(it != assignedTasks.end()){
            
            runningTasks.insert(std::pair<unsigned, _task*>(task.id(), it->second));
            
            assignedTasks.erase(it);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "assigned task not found: " << task;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
    void finishTask(const _task& task){
        
        std::map<unsigned, _task*>::const_iterator it = runningTasks.find(task.id());
        
        if(it != runningTasks.end()){
            
            finishedTasks.insert(std::pair<unsigned, _task*>(task.id(), it->second));
            
            runningTasks.erase(it);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "running task not found: " << task;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
    void setMoving(const _agent& agent, const _stepPath& path);
    
    _stepMap& getStepMap() const {
        return stepMap;
    }

    const _map& getMap() const {
        return map;
    }
    
    const _endpointsDistanceAlgorithm& getEndpointsDistanceAlgorithm() const {
        return endpointsDistanceAlgorithm;
    }
    
    bool anyPendingTask()const{
        return pendingTasks.empty();
    }
    
    bool anyAssignedTask()const{
        return assignedTasks.empty();
    }
    
    bool anyRunningTask()const{
        return runningTasks.empty();
    }
        
    void listAgents(const std::function<bool(_agent&)> function)const;
    
    void listEndpoints(const std::function<bool(const _site&)>& function) const{
        
        for (auto ep : endpoints) {
            
            if(function(ep)) return;

        }

        
    }
    
    void listChargingEndpoints(const std::function<bool(const _site&)>& function) const{
        
        for (auto ep : chargingEndpoints) {
            
            if(function(ep)) return;

        }

        
    }
    
    void listPendingTasks(const std::function<bool(const _task&)> function)const{
        
        for (auto taskPair : pendingTasks) {
            
            if(function(*taskPair.second))return;

        }

    }
    
    void listTaskDeliveryEndpoints(const std::function<bool(const _site&)> function)const{
        
        for (auto pendpoint : taskDeliveryEndpoints) {
            
            if(function(pendpoint.second))return;

        }

    }
    
    void listNonTaskDeliveryEndpoints(const std::function<bool(const _site&)> function)const{
        
        for (auto pendpoint : nonTaskDeliveryEndpoints) {
            
            if(function(pendpoint.second))return;

        }

    }
    
    bool isTaskDeliveryEndpoint(const _site& site) const {
        
        std::map<unsigned, _site>::const_iterator it = taskDeliveryEndpoints.find(site.linearIndex(map.getColumn_size()));
        
        return it != taskDeliveryEndpoints.cend();
        
    }
    
    bool isNonTaskDeliveryEndpoint(const _site& site)const{
        
        std::map<unsigned, _site>::const_iterator it = nonTaskDeliveryEndpoints.find(site.linearIndex(map.getColumn_size()));
        
        return it != nonTaskDeliveryEndpoints.cend();
        
    }
       
    unsigned getCurrentStep() const {
        return currentStep;
    }
    
    void reportTaskUpdate(const _agent& agent, const _task& task, ReportTask::PathType pathType, const _stepPath& path){
        
        reportTaskMap.reportTaskUpdate(agent, task, pathType, path);
        
    }
    
    int currentEnergy()const;
    int energyExpenditure()const;
    
    unsigned getPendingTaskAmount()const{
        return pendingTasks.size();
    }
    
    unsigned getFinishedTaskAmount()const{
        return finishedTasks.size();
    }
    
    friend std::ostream& operator<<(std::ostream& os, const _token& obj) {
        os << "current step: " << obj.currentStep << std::endl;
        os  << std::endl << "energy: " << obj.currentEnergy() << std::endl;
        os  << std::endl << "agents: " << std::endl;
        for (auto elem : obj.agents) {
            os << *elem.second <<  std::endl;
        }        
        os  << std::endl << "reportTaskMap: " << std::endl << obj.reportTaskMap;
        
        return os;
    }

    virtual void draw(const Render& render) const;
    
    virtual _token::TokenUpdateType updatePath(_agent& agent) = 0;
    virtual _token::TokenUpdateType updateChargingPath(_agent& agent) = 0;
    
    virtual _token::TokenUpdateType updateTrivialPathToAgent(_agent& agent);    
    virtual _token::TokenUpdateType updateChargingTrivialPathToAgent(_agent& agent);
    
    
    void error_site_collision_check() const;
    void error_edge_collision_check() const;
    
    int getOneTaskId(){
        return c_task_ids--;
    }
    
    int giveBackOneTaskId(){
        return c_task_ids++;
    }
    
    void addPendingTask(const _task& task){
        this->pendingTasks.insert(std::pair<unsigned, _task*>(task.id(), task.instance()));
        this->reportTaskMap.addTask(task, currentStep);
        
        this->taskDeliveryEndpoints.insert(std::pair<unsigned, _site>(task.getDelivery().linearIndex(map.getColumn_size()), task.getDelivery()));  
//        this->taskDeliveryEndpoints.insert(std::pair<unsigned, _site>(task.getPickup().linearIndex(map.getColumn_size()), task.getPickup()));
        
        this->nonTaskDeliveryEndpoints.erase(task.getDelivery().linearIndex(map.getColumn_size()));    
//        this->nonTaskDeliveryEndpoints.erase(task.getPickup().linearIndex(map.getColumn_size()));
    }  
    
    void addBackwardTask(unsigned step, const _task& task) {
        
        std::map<unsigned, std::vector<_task*>>::iterator it;
        it = this->backwardTasks.find(step);

        if (it != this->backwardTasks.end()) {

            (*it).second.push_back(task.instance());

        } else {
            
            auto it2 = this->backwardTasks.insert(std::pair<unsigned, std::vector<_task*>>(step, std::vector<_task*>()));
            
            it2.first->second.push_back(task.instance());
            
        }

    }

private:
    
    friend class _system;
    
    void addAgent(const _agent& agent){
        this->agents.insert(std::pair<unsigned, _agent*>(agent.id(), agent.instance()));
    }
        
    
    
    void stepping() {        
        reinsert_c_tasks(currentStep++);
    } 
               
private:
    unsigned currentStep = 0;
    int c_task_ids = -1;
    
    const _map& map;
    _stepMap& stepMap;
    
    std::vector<_site> &endpoints, &chargingEndpoints;    
    std::map<unsigned,_site> nonTaskDeliveryEndpoints, taskDeliveryEndpoints;    
    const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm;
    
    std::map<unsigned, std::vector<_task*>> backwardTasks;//step->task
    std::map<unsigned, _agent*> agents;
    std::map<unsigned, _task*> pendingTasks, assignedTasks, runningTasks, finishedTasks;
    std::map<unsigned, unsigned> assignTaskAgent; // task.id()->agent.id()
    ReportTaskMap reportTaskMap;
    
    
    
    void reinsert_c_tasks(unsigned step) {
        
        std::map<unsigned, std::vector<_task*>>::iterator it;
        it = this->backwardTasks.find(step);

        if (it != this->backwardTasks.end()) {

            for (auto task : it->second) {
                
                addPendingTask(*task);

            }
            
            this->backwardTasks.erase(it);            

        } 

    }
    
};

#endif /* _TOKEN_H */

