/* 
 * File:   _ga_agent.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 07:28
 */

#ifndef _GA_AGENT_H
#define _GA_AGENT_H

#include "_task.h"
#include "_stepAstarAlgorithm.h"
#include "Identifiable.h"
#include "_ga_agent_state.h"


class _task;
class _stepSite;
class _stepPath;
class _ga_agent_state;

class _ga_agent : public Identifiable<int>, public Drawable{
    
public:
    
    _ga_agent(
        int id, 
        const _stepSite&,
        const _agent_energy_system&);   
    
    _ga_agent(const _ga_agent&);    
    
    virtual ~_ga_agent();
    
    int id()const;
    virtual void draw(const Render& render) const;
    
    bool isTaskAssigned()const;
    bool isDelivering()const;
    bool isPickuping()const;
    bool isAtTrivialPath()const;
    bool isAtEnergyChargedLevel()const;        
    bool isAtEnergyCriticalLevel()const;    
    bool isAtEnergyDeadLevel()const;
    
    _stepSite getGoalSite()const; 
    _stepSite getCurrentSite()const;
    _stepSite getFutureSite()const; 
    const _agent_energy_system& getAgentEnergySystem() const;

    void stepping(const _map& map);
    
    bool updateTaskPath(const _stepPathAlgorithm& astar, _stepMap& map, const _task* const task);
    
    friend std::ostream& operator<<(std::ostream& os, const _ga_agent& obj);

        
private:
    
    friend class _ga_token;
       
    const _task* getCurrentTask() const;
    void assignTask(const _task& task);
    void unassignTask();
    void assignPath(const _stepPath& path);
    
private:
    friend class _ga_agent_state;
    friend class _ga_agent_state_free;
    friend class _ga_agent_state_buzy;
    void change_state(_ga_agent_state*);
    _stepPath path;
    _agent_energy_system agent_energy_system;
    
private:
    int _id;
    _ga_agent_state* state = nullptr;
    _task* currentTask = nullptr;
//    _stepPath path;
//    _agent_energy_system agent_energy_system;

};

#endif /* _GA_AGENT_H */

