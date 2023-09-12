/* 
 * File:   _agent.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 09:30
 */

#ifndef _AGENT_H
#define _AGENT_H

#include <climits>
#include <sstream>
#include "Identifiable.h"
#include "_stepMap.h"
#include "_stepPath.h"
#include "_task.h"
#include "_agent_state.h"
#include "Drawable.h"
#include "_agent_energy_system.h"


class _token;
class _system;
class _agent_state;
class _agent : public Identifiable<int>, public Drawable{
public:
        
    _agent(int _id, const _stepSite& currentSite, int energyCurrentLevel, int energyMaximumLevel, int energyChargingLevel, int energyCriticalLevel);
    
    _agent(const _agent& other);

    virtual ~_agent();
    
    int id() const {
        return _id;
    }
        
    void setPath(const _stepPath& path){
        
        this->_currentPath = path;
        
    }
    
    void assignTask(const _task& task, const _stepPath& path){
        this->_currentPath = path;
        tasks.push_back(task);
        _currentTaskIndex = tasks.size() - 1;
    }
    
    void unassignTask(){
        _currentTaskIndex = -1;
    }
    
    virtual _agent* instance()const{
        return new _agent(*this);
    }
    
    bool isAbleToFulfillTaskPath(const _map& map, const _task& task, const _stepPath& taskPath)const{
        int need = this->energy_system.appraiseTaskPath(map, task, taskPath);
        return need + 10  /*+ this->energy_system.criticalLevel()/2*/ < this->energy_system.currentLevel();
    }
    
    bool isAbleToFulfillChargingTaskPath(const _map& map, const _task& task, const _stepPath& taskPath)const{
        int need = this->energy_system.appraiseTaskPath(map, task, taskPath);
        return need + 10  < this->energy_system.currentLevel();
    }
        
    bool isAbleToFulfillNoCarryngPath(const _map& map, const _stepPath& path)const{
        int need = this->energy_system.appraiseNoCarryngPath(map, path);
        return need + 10 < this->energy_system.currentLevel();
    }
     
    
    const _stepPath& currentPath()const{
        return _currentPath;
    }
 
    const _stepSite& currentSite()const{
        
        return _currentPath.currentSite();
        
    }
    
    const _stepSite& goalSite() const{
        
        return _currentPath.goalSite();
        
    }
    
    const bool isInFinishedPath()const{
        return _currentPath.size() == 1;
    }
    
    const bool isAwaitingTask()const{
        
        return _currentTaskIndex == -1;
        
    }
        
    const bool isAssigned()const{
        
        return _currentTaskIndex > -1;// && !_charging_task;
        
    }
    
    const bool isDelivering()const{
        
        if(_currentTaskIndex > -1){
            
            return isInFinishedPath() && currentTask().getDelivery().match(_currentPath.goalSite());
            
        }
        
        return false;
        
    }
    
    const bool isChargingDelivering()const{
        
        if(_currentTaskIndex > -1 && !currentTask().getDelivery().match(_currentPath.goalSite())){
            
            return !isInFinishedPath() &&  currentTask().getDelivery().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    const bool isPickupping()const{
        
        if(_currentTaskIndex > -1){
            
            return currentTask().getPickup().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
       
    virtual void draw(const Render& render) const;    
    
    friend std::ostream& operator<<(std::ostream& os, const _agent& obj) {
        os <<"agent id: "<< obj._id << std::endl;
        os << "energy " << obj.energy_system << std::endl;
        os <<"agent path: "<< obj._currentPath << std::endl;
        if(obj._currentTaskIndex > -1)
            os <<"agent current task: "<< obj.currentTask() << std::endl;
        os <<"agent state: "<< obj._state->stateName() << std::endl;
        return os;
    }
    
    const _task& currentTask() const{
        
        if(_currentTaskIndex > -1)
            
            return tasks.at(_currentTaskIndex);
        
        try {
            std::ostringstream stream;
            stream << "no current task" << std::endl;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
                
    }
    
    
    
    bool isAtEnergyChargedLevel()const{
        return energy_system.isAtChargedLevel();
    }
        
    bool isAtEnergyCriticalLevel()const{
        return energy_system.isAtCriticalLevel();
    }  
    
    bool isAtEnergyDeadLevel()const{
        return energy_system.isAtDeadLevel();
    }

    bool staysInSamePlace() const {
        return currentSite().match(_previousSite);
    }
    
    void expendCarryngStepping(bool isChargingSite){
        
        energy_system.expendCarryngStepping(isChargingSite, _currentPath.currentSite(), _currentPath.futureSite());
        
    }
    
    void expendNoCarryngStepping(bool isChargingSite){
        
        energy_system.expendNoCarryngStepping(isChargingSite, _currentPath.currentSite(), _currentPath.futureSite());
        
    } 
    
    AES energyState()const{
        
        return energy_system.energyState();
        
    }
      
protected:
    const int _id;
    _agent_state* _state;
    _stepPath _currentPath;    
    std::vector<_task> tasks;
    int _currentTaskIndex = -1;
    _agent_energy_system energy_system;
    _stepSite _previousSite;
            
private:
    friend class _system;
    virtual void move(_system& system);
    virtual void receive(_system& system);
    
private:
    
    friend class _agent_state;
    
    virtual void expendEnergy(_token& token); 
    virtual void stepping(_token& token);
    
    void changeState(_agent_state* state){
        _state = state;
    }
        
    _stepSite previousSite() const {
        return _previousSite;
    }
    
    void stepping(){
        
        if (_currentPath.size() > 1) {

            this->_previousSite = _currentPath.pop();   

        } else {
        
            try {
                std::ostringstream stream;
                stream << "invalid path size - agent: " << std::endl << *this;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
        
        }
       
    }
    
private:       
    
    friend class _token;
    
    int currentEnergyLevel() const {
        return energy_system.currentLevel();
    }
    
    int energyCharging() const {
        return energy_system.getCharging();
    }
    
    
        
};

#endif /* _AGENT_H */

