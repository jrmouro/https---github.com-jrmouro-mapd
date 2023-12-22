/* 
 * File:   _r_ga_token.h
 * Author: ronaldo
 *
 * Created on 22 de dezembro de 2023, 06:08
 */

#ifndef _R_GA_TOKEN_H
#define _R_GA_TOKEN_H

#include <map>
#include <functional>
#include "_stepPathAlgorithm.h"
#include "Identifiable.h"
#include "Drawable.h"
#include "Writable.h"
#include "_ga_agent.h"

#include "./../../../../../NetBeansProjects/Map/_r_map.h"
#include "_r_stepMap.h"

class _task;
class _stepPath;
class _ga_agent;
class __stepPathAlgorithm;

using Int_to_Int = _r_map<int,int>;
using unsigned_to_Site = _r_map<unsigned,_site>;
using Int_to_Task = _r_map<int,_task>;

class _r_ga_token  : public Identifiable<std::string>, public Drawable, public Writable, public Recoverable{

    _r_ga_token(
            const _map& map, 
            const _r_stepMap& stepMap,
            const _agent_energy_system& agent_energy_system);  
    
    _r_ga_token(const _r_ga_token& other);   
    virtual ~_r_ga_token();
    
    virtual _r_ga_token* getClone()const;
    
    virtual std::string id() const; 
    
    virtual const std::string& getName() const;      
    void setName(const std::string& name);
    
    int energyExpenditure() const;
    
    virtual void draw(const Render&) const;
    
    void addPendingTask(const _task&);
    void listNonTaskEndpoints(const std::function<bool(const _site&)>& function)const;
    
    virtual bool updateAgentTaskPath(int, int);
    
            
    const _task* getPendingTaskById(int)const;
    void listPendingTasks(const std::function<bool(unsigned, const _task&)>& function) const;    
    void listAgents(const std::function<bool(_ga_agent&)>& function);
    void listConstAgents(const std::function<bool(unsigned, const _ga_agent&)>& function) const;
    
    virtual bool isIdle() const;
    virtual bool isLate() const;
    
    const _ga_agent& getAgentById(int agentId) const;
    
    bool isAgentDelivering(int agentId)const;
    bool isAgentPickuping(int agentId)const;
    bool isAgentAtTrivialPath(int agentId)const;
    
    
    virtual void writeHeader(std::ostream& fs) const;    
    virtual void writeRow(std::ostream& fs) const;   
    
    unsigned getCurrentStep() const;    
    void stepping();    
    unsigned numberOfpendingTasks()const;    
    unsigned numberOfAgents()const;    
    const _map& getMap() const;
    const _r_stepMap& getStepMap() const;

    const _agent_energy_system& getAgent_energy_system() const;
    
    unsigned getRunningTaskAmount()const;
    
    unsigned getAssignedTaskAmount()const;
    
    unsigned getPendingTaskAmount()const;
    
    unsigned getFinishedTaskAmount()const;
    
    void error_site_collision_check() const;
    void error_edge_collision_check() const;
    
    unsigned GetMaxPlannedStep() const;
    
    virtual void save(){     
        
        if(r_name == nullptr){
            
            r_name = new std::string(_name);
            
            for (auto elem : agents) {
                
                elem.second.save();

            }
            
            stepMap.save();
            nonTaskEndpoints.save();
            pendingTasks.save(); 
            assignedTasks.save();
            runningTasks.save();
            finishedTasks.save();    
            assignTaskAgent.save();
            
            r_currentStep = currentStep;

            
        }
                
    }
    
    virtual void unsave(){
        
        if(r_name != nullptr){
            
            r_name = nullptr;
            
            for (auto elem : agents) {
                
                elem.second.unsave();

            }
            
            stepMap.unsave();            
            nonTaskEndpoints.unsave();
            pendingTasks.unsave(); 
            assignedTasks.unsave();
            runningTasks.unsave();
            finishedTasks.unsave();    
            assignTaskAgent.unsave();
                        
        }
        
    }
    
    virtual void restore(){
        
        if(r_name != nullptr){
            
            delete r_name;
            r_name = nullptr;
            
            for (auto elem : agents) {
                
                elem.second.restore();

            }
            
            stepMap.restore();            
            nonTaskEndpoints.restore();
            pendingTasks.restore(); 
            assignedTasks.restore();
            runningTasks.restore();
            finishedTasks.restore();    
            assignTaskAgent.restore();
            
            currentStep = r_currentStep;
            
        }
        
    }
    
protected:
    
    
    const _agent_energy_system agent_energy_system;
    const _map& map;
    
    std::map<int, _ga_agent> agents;
    
    _r_stepMap stepMap;
    unsigned_to_Site nonTaskEndpoints;
    Int_to_Task pendingTasks, assignedTasks, runningTasks, finishedTasks;    
    Int_to_Int assignTaskAgent;  
    
    
    std::string _name = "GAT", *r_name = nullptr;
    unsigned currentStep = 0, r_currentStep;
    
    bool updateAgentRestPathCloserEndpoint(_ga_agent&, const _site&);    
    virtual bool updateAgentTaskPath_pendingTask(_ga_agent&, int, bool);    
    virtual bool updateAgentTaskPath_resting(_ga_agent&, int);
    virtual bool updateAgentTaskPath_going_to_resting(_ga_agent&, int);
    virtual bool updateAgentTaskPath_pickuping(_ga_agent&, int);
    virtual bool updateAgentTaskPath_going_to_pickuping(_ga_agent&, int);
    virtual bool updateAgentTaskPath_going_to_rest_pickuping(_ga_agent&, int);
    virtual bool updateAgentTaskPath_going_to_rest_pickuping_to_pickuping(_ga_agent&, int);
    virtual bool updateAgentTaskPath_delivering(_ga_agent&, int);
    virtual bool updateAgentTaskPath_rest_pickuping(_ga_agent&, int);
    virtual bool updateAgentTaskPath_rest_delivering(_ga_agent&);
    virtual bool updateAgentTaskPath_going_to_rest_delivering_to_delivering(_ga_agent&, int);  
    _ga_agent* getEndpointObstructor(const _ga_agent& agent, const _site& endpoint);    
    bool liberateEndpoint(const _ga_agent& agent, const _site& endpoint);
    
};

#endif /* _R_GA_TOKEN_H */

