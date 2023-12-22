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
#include "Recoverable.h"
#include "_ga_agent_state.h"


class _task;
class _stepSite;
class _stepPath;
class _ga_agent_state;

class _ga_agent : public Identifiable<int>, public Drawable, public Recoverable{
    
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
    
    bool isGoingToDelivering()const;
    bool isGoingToPickuping()const;
    bool isGoingToResting()const;
    bool isGoingToRestPickuping()const;
    bool isGoingToRestDelivering()const;
    
    bool isAtResting()const;
    bool isAtDelivering()const;
    bool isAtPickuping()const;
    bool isAtRestPickuping()const;
    bool isAtRestDelivering()const;
    
    
    bool isAtTrivialPath()const;
    bool isAtEnergyChargedLevel()const;        
    bool isAtEnergyCriticalLevel()const;    
    bool isAtEnergyDeadLevel()const;
    
    const _stepSite& goalSite()const; 
    const _stepSite& currentSite()const;
    const _stepSite& getFutureSite()const; 
    const _agent_energy_system& getAgentEnergySystem() const;

    void stepping(const _map& map);
    
    bool updateTaskPath(const _stepPathAlgorithm& astar, _stepMap& map, const _task* const task);
    
    friend std::ostream& operator<<(std::ostream& os, const _ga_agent& obj);
    
    bool isFree()const;    
    bool isBuzy()const;
    
    void setStateFree();
    void setStateBuzy();
    
    const _stepSite& previousSite() const {
        return _previousSite;
    }
        
    const _task* getCurrentTask() const;
        
    const _stepPath& getPath() const;
    
    virtual void save(){
        
        if(r_path == nullptr){
        
            r_path = new _stepPath(_path);
            r_energy_system_level = agent_energy_system.currentLevel();
            r_energy_system_charging = agent_energy_system.getCharging();
            r_state = _state;
            r_currentTask = _currentTask;
            r_previousSite = new _stepSite(_previousSite);
        
        }
        
    }
    
    virtual void unsave(){
        
        if(r_path != nullptr){
        
            delete r_path;
            delete r_previousSite;
            
            r_path = nullptr;
            r_previousSite = nullptr;
            r_state = nullptr;
            r_currentTask = nullptr;
        
        }
        
    }
    
    virtual void restore(){
        
        if(r_path != nullptr){
        
            _path = *r_path;
            delete r_path;
            
            agent_energy_system.setCharging(r_energy_system_charging);
            agent_energy_system.setCurrent_level(r_energy_system_level);
            
            _state = r_state;
            
            _currentTask = r_currentTask;
                        
            _previousSite = *r_previousSite;
            delete r_previousSite;
        
        }
        
    }
    
private:
    
    friend class _r_ga_token;
    friend class _ga_token;
    friend class _ga_token_p;
       
    
    void assignTask(const _task& task);
    void unassignTask();
    void assignPath(const _stepPath& path);
    void progressPath(const _stepPath& path);
    
    
    
private:
    friend class _ga_agent_state;
    friend class _ga_agent_state_free;
    friend class _ga_agent_state_buzy;
    void change_state(_ga_agent_state*);
    _stepPath _path;
    _agent_energy_system agent_energy_system;
    
private:
    int _id;
    _ga_agent_state* _state = nullptr;
    _task* _currentTask = nullptr;
    _stepSite _previousSite;
    
    // recoverable
    
    _stepPath *r_path = nullptr;
    int r_energy_system_level, r_energy_system_charging;
    _ga_agent_state* r_state = nullptr;
    _task* r_currentTask = nullptr;
    _stepSite *r_previousSite = nullptr;
    

};

#endif /* _GA_AGENT_H */

