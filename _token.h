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
#include "_c_task.h"
#include "ReportTask.h"
#include "MapdException.h"
#include "ManhattanAlgorithm.h"
#include "_endPointsDistanceAlgorithm.h"
#include "_stepAstarAlgorithm.h"
#include "_agent.h"

class _agent;
class _token : public Drawable{
public:
    
    _token(const _map& map, _stepMap& stepMap, std::vector<_site>& endpoints, const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm):
    map(map),
    stepMap(stepMap),
    endpoints(endpoints),
    endpointsDistanceAlgorithm(endpointsDistanceAlgorithm){}            
    
    _token(const _token& other) :
    map(other.map),
    stepMap(other.stepMap),
    endpoints(other.endpoints),
    endpointsDistanceAlgorithm(other.endpointsDistanceAlgorithm),
    assignTaskAgent(other.assignTaskAgent),
    reportTasks(other.reportTasks), 
    currentStep(other.currentStep) { 
        
        for (auto p_c_task : c_tasks) {
            
            std::vector<_task*> vtask;
            
            for (auto elem : p_c_task.second) {
                
                vtask.push_back(elem->instance());

            }

            c_tasks.insert(std::pair<unsigned, std::vector<_task*>>(p_c_task.first, vtask));
            
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

    
    virtual ~_token(){ 
        
        for (auto p_c_task : c_tasks) {
                        
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
    
    void assignTask(const _task& task, const _agent& agent){
        
        std::map<unsigned, _task*>::const_iterator it = pendingTasks.find(task.id());
        
        if(it != pendingTasks.end()){
            
            assignTaskAgent.insert(std::pair<unsigned, unsigned>(task.id(), agent.id()));
            assignedTasks.insert(std::pair<unsigned, _task*>(task.id(), it->second));
            
            pendingTasks.erase(task.id());
            
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
            
            assignedTasks.erase(task.id());
            
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
            
            runningTasks.erase(task.id());
            
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
    
    std::vector<_site>& getEndpoints() const {
        return endpoints;
    }

    _stepMap& getStepMap() const {
        return stepMap;
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
    
    void listPendingTasks(const std::function<bool(const _task&)> function)const{
        
        for (auto taskPair : pendingTasks) {
            
            if(function(*taskPair.second))return;

        }

    }
    
    void listPending_ptr_Tasks(const std::function<bool(_task*)> function)const{
        
        for (auto taskPair : pendingTasks) {
            
            if(function(taskPair.second))return;

        }

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
    
    void updatePath(_agent& agent);
    
    void updateTrivialPathToAgent(_agent& agent);
        
    bool isConflictingSiteWithAnyTaskDelivery(const _site& site, _task& conflitTask) const;
    bool selectNewRestEndpointToAgent(const _agent& agent, _site& selectNewSite) const;
    bool selectNewRestEndpointPathToAgent(const _agent& agent, _task& conflictTask, _stepPath& restPath) const;
    bool updateRestPathToAgent(_agent& agent);
    
    bool selectNewTaskToAgent(const _agent& agent, _task& selectedTask) const;
    bool taskPathToAgentToAgent(const _agent& agent, const _task& task, _stepPath& path) const;
    bool selectNewTaskPathToAgent(const _agent& agent, _task& selectedTask, _stepPath& path) const;
    bool updateTaskPathToAgent(_agent& agent);
    
    
    
    bool selectNewTaskPathToAgent(const _agent& agent, _task& origTask, _c_task& c_task, _stepPath& path, _stepPath& c_path, bool& c_taskFlag, float threshold) const;
    
private:
    
    friend class _system;
    
    void addAgent(const _agent& agent){
        this->agents.insert(std::pair<unsigned, _agent*>(agent.id(), agent.instance()));
    }
        
    void addPendingTask(const _task& task){
        this->pendingTasks.insert(std::pair<unsigned, _task*>(task.id(), task.instance()));
        this->reportTasks.insert(std::pair<unsigned, ReportTask>(task.id(), ReportTask(task, currentStep)));
    }
    
    void stepping() {
        this->currentStep++;
    } 
            
private:
    int c_task_ids = -1;
    const _map& map;
    _stepMap& stepMap;
    std::vector<_site>& endpoints;
    const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm;
    
    std::map<unsigned, std::vector<_task*>> c_tasks;//step->task
    std::map<unsigned, _agent*> agents;
    std::map<unsigned, _task*> pendingTasks, assignedTasks, runningTasks, finishedTasks;
    std::map<unsigned, unsigned> assignTaskAgent; // task.id()->agent.id()
    std::map<unsigned, ReportTask> reportTasks;
    unsigned currentStep = 0;
    
    int new_C_TaskId(){
        return c_task_ids--;
    }
    
    void add_c_task(unsigned step, const _c_task& c_task) {
        
        std::map<unsigned, std::vector<_task*>>::iterator it;
        it = this->c_tasks.find(step);

        if (it != this->c_tasks.end()) {

            (*it).second.push_back(c_task.instance());

        } else {
            
            auto it2 = this->c_tasks.insert(std::pair<unsigned, std::vector<_task*>>(step, std::vector<_task*>()));
            
            it2.first->second.push_back(c_task.instance());
            
        }

    }
    
    void reinsert_c_tasks(unsigned step) {
        
        std::map<unsigned, std::vector<_task*>>::iterator it;
        it = this->c_tasks.find(step);

        if (it != this->c_tasks.end()) {

            for (auto task : it->second) {
                
                addPendingTask(*task);

            }
            
            this->c_tasks.erase(it);            

        } 

    }
    
};

#endif /* _TOKEN_H */

