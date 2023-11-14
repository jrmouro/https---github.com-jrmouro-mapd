/* 
 * File:   _ga_token.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 08:51
 */

#ifndef _GA_TOKEN_H
#define _GA_TOKEN_H

#include <map>
#include <functional>
#include "_stepPathAlgorithm.h"
#include "Identifiable.h"
#include "Drawable.h"
#include "Writable.h"
#include "_ga_agent.h"

class _task;
class _stepPath;
class _ga_agent;
class __stepPathAlgorithm;

class _ga_token  : public Identifiable<std::string>, public Drawable, public Writable{
public:
    
    _ga_token(
            const _map& map, 
            const _stepMap& stepMap,
            const _agent_energy_system& agent_energy_system);  
    
    _ga_token(const _ga_token& other);   
    virtual ~_ga_token();
    
    virtual _ga_token* getClone()const;
    
    virtual std::string id() const; 
    
    virtual const std::string& getName() const;      
    void setName(const std::string& name);
    
    int energyExpenditure() const;
    
    virtual void draw(const Render&) const;
    
    void addPendingTask(const _task&);
    void listNonTaskEndpoints(const std::function<bool(const _site&)> function)const;
    
    virtual bool updateAgentTaskPath(int, int);
    
            
    const _task* getPendingTaskById(int)const;
    void listPendingTasks(const std::function<bool(unsigned, const _task&)> function) const;    
    void listAgents(const std::function<bool(_ga_agent&)> function);
    void listConstAgents(const std::function<bool(unsigned, const _ga_agent&)> function) const;
    
    virtual bool isIdle() const;
    
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
    const _stepMap& getStepMap() const;

    const _agent_energy_system& getAgent_energy_system() const;
    
    unsigned getRunningTaskAmount()const;
    
    unsigned getAssignedTaskAmount()const;
    
    unsigned getPendingTaskAmount()const;
    
    unsigned getFinishedTaskAmount()const;
    
    void error_site_collision_check() const;
    void error_edge_collision_check() const;
    
    unsigned GetMaxPlannedStep() const;
    
protected:
    std::string _name = "GAT";
    const _map& map;
    _stepMap stepMap;
    std::map<unsigned, _site> nonTaskEndpoints;
    std::map<int, _task> pendingTasks, assignedTasks, runningTasks, finishedTasks;
    std::map<int, _ga_agent> agents;
    std::map<int, int> assignTaskAgent;
    const _agent_energy_system agent_energy_system;
    unsigned currentStep = 0;
    
//    bool updateAgentRestPath(_ga_agent&);
    bool updateAgentRestPathCloserEndpoint(_ga_agent&, const _site&);
//    bool updateAgentPickupRestPath(_ga_agent&);
    
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

#endif /* _GA_TOKEN_H */

