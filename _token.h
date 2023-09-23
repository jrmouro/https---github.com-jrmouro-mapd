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
#include "_manhattanAlgorithm.h"
#include "_endpointsDistanceAlgorithm.h"
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
        none
    };
    
     _token(
            const _map& map, 
            const _stepMap& stepMap,
            const _agent_energy_system& agent_energy_system):
                    map(map),
                    stepMap(stepMap),
                    agent_energy_system(agent_energy_system),
                    nonTaskDeliveryEndpoints(),
                    taskDeliveryEndpoints(){
        
        map.listEndpoints([this, map](const _site& endpoint){
            
            nonTaskDeliveryEndpoints.insert(std::pair<unsigned, _site>(
                endpoint.linearIndex(map.getColumn_size()), 
                endpoint));
            
            return false;
        });
        
        map.listBotsEndPoints([this, agent_energy_system](unsigned botId, const _site& endpoint){
            
            agents.insert(
                std::pair<unsigned, _agent*>(
                    botId, 
                    new _agent(
                        botId, 
                        _stepSite(0, endpoint.GetRow(), endpoint.GetColunm()), 
                        agent_energy_system)));
            
            return false;
            
        });
    
    }            
    
    virtual ~_token(){} 
    
    virtual void writeHeader(std::ostream& fs) const {
        Writable::strWrite(*this, fs, "token_id", true); 
        Writable::strWrite(*this, fs, "token_name", true); 
        Writable::strWrite(*this, fs, "current_step", true);
        Writable::strWrite(*this, fs, "energy system id", true);
        Writable::strWrite(*this, fs, "energy expenditure", true);
        Writable::strWrite(*this, fs, "finished_tasks", false);
    }   
    
    virtual void writeRow(std::ostream& fs) const {
        Writable::strWrite(*this, fs, id(), true); 
        Writable::strWrite(*this, fs, name(), true); 
        Writable::uintWrite(*this, fs, currentStep, true);
        Writable::strWrite(*this, fs, agent_energy_system.id(), true);
        Writable::intWrite(*this, fs, energyExpenditure(), true);
        Writable::uintWrite(*this, fs, finishedTasks.size(), false);
    }
        
    virtual std::string id() const = 0;
    virtual std::string name() const = 0;
    
    bool isChargingSite(const _agent& agent)const{
        
        return isChargingSite(agent.currentSite());
        
    }
    
    bool isChargingSite(const _site& site)const{
        
        return map.getTypeOfSite(site) == _map::TypeOfSite::bot;
        
    }
        
    void assignTask(const _task& task, const _agent& agent){
        
        std::map<int, _task>::const_iterator it = pendingTasks.find(task.id());
        
        if(it != pendingTasks.end()){
            
            assignTaskAgent.insert(std::pair<int, int>(task.id(), agent.id()));
            assignedTasks.insert(std::pair<int, _task>(task.id(), it->second));
            
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
        
        std::map<int, _task>::const_iterator it = assignedTasks.find(task.id());
        
        if(it != assignedTasks.end()){
            
            runningTasks.insert(std::pair<int, _task>(task.id(), it->second));
            
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
        
        std::map<int, _task>::const_iterator it = runningTasks.find(task.id());
        
        if(it != runningTasks.end()){
            
            finishedTasks.insert(std::pair<int, _task>(task.id(), it->second));
            
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
    
    const _stepMap& getStepMap() const {
        return stepMap;
    }

    const _map& getMap() const {
        return map;
    }
    
    const _endpointsDistanceAlgorithm& getEndpointsDistanceAlgorithm() const {
        return map.getEndpointsDistanceAlgorithm();
    }
    
    bool anyBackwardTask()const{
        return !backwardTasks.empty();
    }
    
    bool anyPendingTask()const{
        return !pendingTasks.empty();
    }
    
    bool anyAssignedTask()const{
        return !assignedTasks.empty();
    }
    
    bool anyRunningTask()const{
        return !runningTasks.empty();
    }
        
    void listAgents(const std::function<bool(_agent&)> function)const;
    
    void listEndpoints(const std::function<bool(const _site&)>& function) const{
        
        map.listEndpoints(function);
        
    }
    
    void listChargingEndpoints(const std::function<bool(const _site&)>& function) const{
        
        map.listBotsEndPoints(function);
        
    }
    
    void listPendingTasks(const std::function<bool(const _task&)> function)const{
        
        for (auto taskPair : pendingTasks) {
            
            if(function(taskPair.second))return;

        }

    }
    
    void listAssignedTasks(const std::function<bool(const _task&)> function)const{
        
        for (auto taskPair : assignedTasks) {
            
            if(function(taskPair.second))return;

        }

    }
    
    void listNoRunningYetTasks(const std::function<bool(const _task&, const _agent*)> function)const{
        
        for (auto taskPair : pendingTasks) {
            
            if(function(taskPair.second, nullptr))return;

        }
        
        for (auto taskPair : assignedTasks) {
            
            if(function(taskPair.second, assignmentTaskAgent(taskPair.second)))return;

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
    
    unsigned getRunningTaskAmount()const{
        return runningTasks.size();
    }
    
    unsigned getAssignedTaskAmount()const{
        return assignedTasks.size();
    }
    
    unsigned getBackwardTaskAmount()const{
        
        unsigned count = 0;
        
        for (auto elem : backwardTasks) {

            count += elem.second.size();
            
        }
        
        return count;
        
    }
    
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
        
        os << obj.stepMap << std::endl;
        os  << std::endl << "reportTaskMap: " << std::endl << obj.reportTaskMap;
        
        return os;
    }

    virtual void draw(const Render& render) const;
    
    virtual _token::TokenUpdateType updatePath(_agent& agent) = 0;
    virtual _token::TokenUpdateType updateChargingPath(_agent& agent) = 0;
    
    virtual _token::TokenUpdateType updateTrivialPathToAgent(_agent& agent);    
//    virtual _token::TokenUpdateType updateChargingTrivialPathToAgent(_agent& agent);
    
    
    void error_site_collision_check() const;
    void error_edge_collision_check() const;
    
    int getOneTaskId(){
        return new_task_ids--;
    }
    
    int giveBackOneTaskId(){
        return new_task_ids++;
    }
    
    void addPendingTask(const _task& task){
        this->pendingTasks.insert(std::pair<unsigned, _task>(task.id(), task));
        this->reportTaskMap.addTask(task, currentStep);
        
        this->taskDeliveryEndpoints.insert(std::pair<unsigned, _site>(task.getDelivery().linearIndex(map.getColumn_size()), task.getDelivery()));  
//        this->taskDeliveryEndpoints.insert(std::pair<unsigned, _site>(task.getPickup().linearIndex(map.getColumn_size()), task.getPickup()));
        
        this->nonTaskDeliveryEndpoints.erase(task.getDelivery().linearIndex(map.getColumn_size()));    
//        this->nonTaskDeliveryEndpoints.erase(task.getPickup().linearIndex(map.getColumn_size()));
    }  
    
    void addBackwardTask(unsigned step, const _task& task) {
        
        std::map<unsigned, std::vector<_task>>::iterator it;
        it = this->backwardTasks.find(step);

        if (it != this->backwardTasks.end()) {

            (*it).second.push_back(task);

        } else {
            
            auto it2 = this->backwardTasks.insert(std::pair<unsigned, std::vector<_task>>(step, std::vector<_task>()));
            
            it2.first->second.push_back(task);
            
        }

    }
    
    virtual bool isIdle()const{
        return  backwardTasks.empty() && 
                pendingTasks.empty() &&
                assignedTasks.empty() &&
                runningTasks.empty();
    }
    
    virtual bool isFinalizedCondition(unsigned lastStepTask)const{
        return currentStep >= stepMap.getStep_size() ||
            (   
                currentStep >= lastStepTask && isIdle()
            );
    }
    
    const ReportTaskMap& getReportTaskMap() const {
        return reportTaskMap;
    }
    
    const _agent_energy_system& getAgent_energy_system() const {
        return agent_energy_system;
    }
    
    
    int idleness() const {
        return map.getNumBots() - (pendingTasks.size() +  assignedTasks.size() + runningTasks.size());
    }


private:
    
    friend class _system;
    
    void addAgent(const _agent& agent){
//        this->agents.insert(std::pair<unsigned, _agent*>(agent.id(), agent.instance()));
        agents.insert(
                std::pair<unsigned, _agent*>(
                    agent.id(), 
                    new _agent(agent.id(), agent.currentSite(), agent_energy_system)));
    }
        
    
    
    void stepping() {   
                                 
        reinsert_c_tasks(currentStep++);
        
    } 
    
    
    _agent* assignmentTaskAgent(const _task& task) const {
        
        std::map<int, int>::const_iterator it = assignTaskAgent.find(task.id());
        
        if(it != assignTaskAgent.end()){
            
            std::map<int, _agent*>::const_iterator it2 = agents.find(it->second);  
            
            if(it2 != agents.end()){
                
                return it2->second;
                
            }
            
        }
        
        return nullptr;
        
    }

               
private:
    
    unsigned currentStep = 0;
    int new_task_ids = -1;    
    const _map& map;
    _stepMap stepMap;        
    _agent_energy_system agent_energy_system;
    
    std::map<unsigned,_site> nonTaskDeliveryEndpoints, taskDeliveryEndpoints;    
    
    std::map<unsigned, std::vector<_task>> backwardTasks;//step->task
    std::map<int, _agent*> agents;
    std::map<int, _task> pendingTasks, assignedTasks, runningTasks, finishedTasks;
    std::map<int, int> assignTaskAgent; // task.id()->agent.id()
    
    ReportTaskMap reportTaskMap;
    
    void reinsert_c_tasks2(unsigned step) {
        
        int idleness = this->idleness();        
        
        std::map<unsigned, std::vector<_task>>::iterator it = backwardTasks.begin();
        std::vector<unsigned> aux;

        for(; it != backwardTasks.end() && it->first < step && idleness > 0; it++){
            
            std::vector<_task>::iterator it2 = it->second.begin();
            
            for(; it2 != it->second.end() && idleness > 0; ){
                                
                addPendingTask(*it2);
                idleness--;
                
                it2 = it->second.erase(it2);
                
            }
            
            if(it->second.empty()) aux.push_back(it->first);           

        }

        for (auto s : aux) {

            backwardTasks.erase(s);

        }       
        
    }
    
    void reinsert_c_tasks(unsigned step) {
        
        if(pendingTasks.empty()){
        
            std::map<unsigned, std::vector<_task>>::iterator it = backwardTasks.begin();
            std::vector<unsigned> aux;

            for(; it != backwardTasks.end(); it++){

                if(it->first < step){

                    aux.push_back(it->first);

                    for (auto task : it->second) {

                        addPendingTask(task);

                    }

                }

            }

            for (auto s : aux) {

                backwardTasks.erase(s);

            }
        
        }

    }
    
};

#endif /* _TOKEN_H */

