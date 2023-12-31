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
        
    _agent(int _id, const _stepSite& currentSite, const _agent_energy_system& agent_energy_system);
        
    _agent(const _agent& other);

    virtual ~_agent();
    
    int id() const {
        return _id;
    }
        
    void setPath(const _stepPath& path){        
        this->_currentPath = path;        
    }
        
    void finishTask(){
        _currentTaskIndex = -1;
    }      
    
    bool isAbleToFulfillTaskPath(const _map& map, const _task& task, const _stepPath& taskPath)const{
        int need = this->energy_system.appraiseTaskPath(map, task, taskPath);
        return this->energy_system.currentLevel() - need > this->energy_system.criticalLevel();
    }
    
    bool isAbleToFulfillChargingTaskPath(const _map& map, const _task& task, const _stepPath& taskPath)const{
        int need = this->energy_system.appraiseTaskPath(map, task, taskPath);
        return need < this->energy_system.currentLevel();
    }
        
    bool isAbleToFulfillNoCarryngPath(const _map& map, const _stepPath& path)const{
        int need = this->energy_system.appraiseNoCarryngPath(map, path);
        return this->energy_system.currentLevel() - need > this->energy_system.criticalLevel();
    }
    
    bool isAbleToFulfillChargingNoCarryngPath(const _map& map, const _stepPath& path)const{
        int need = this->energy_system.appraiseNoCarryngPath(map, path);
        return need < this->energy_system.currentLevel();
    }
    
    bool isAbleToFulfillTrivialPath(const _map& map, const _stepPath& path)const{
        int need = this->energy_system.appraiseNoCarryngPath(map, path);
        return need < this->energy_system.currentLevel();
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
    
    const bool isInGoalSite()const{
        return _currentPath.size() == 1;
    }
    
    const bool isAwaitingTask()const{
        
        return _currentTaskIndex == -1;
        
    }
        
    const bool isAssigned()const{
        
        return _currentTaskIndex > -1;// && !_charging_task;
        
    }
    
    const bool isDeliverySite()const{
        
        if(_currentTaskIndex > -1){
            
            return isInGoalSite() && currentTask().getDelivery().match(_currentPath.goalSite());
            
        }
        
        return false;
        
    }
    
    const bool isChargingDeliverySite()const{
        
        if(_currentTaskIndex > -1 && !currentTask().getDelivery().match(_currentPath.goalSite())){
            
            return !isInGoalSite() &&  currentTask().getDelivery().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    const bool isPickupSite()const{
        
        if(_currentTaskIndex > -1){
            
            return currentTask().getPickup().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
       
    virtual void draw(const Render& render) const;    
    
    friend std::ostream& operator<<(std::ostream& os, const _agent& obj) {
        os <<"agent id: "<< obj._id << std::endl;
        os << "energy " << obj.energy_system << std::endl;
//        os <<"agent path: "<< obj._currentPath << std::endl;
//        if(obj._currentTaskIndex > -1)
//            os <<"agent current task: "<< obj.currentTask() << std::endl;
//        os <<"agent state: "<< obj._state->stateName() << std::endl;
        return os;
    }
    
    const _task& currentTask() const{
        
        if(_currentTaskIndex < 0){
            
            try{
                std::ostringstream stream;
                stream << "no current task" << std::endl;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
        
        }
        
        return tasks.at(_currentTaskIndex);
                
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

    bool stayedInSamePlace() const {
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
    
    bool isFree() const {
        return this->_state->free();
    }
    
    bool isCharging() const {
        return this->_state->charging();
    }
    
    unsigned pickupStepToArrive()const{
        
        if(isAssigned()){
                        
            return _currentPath.stepToArrive(currentTask().getPickup());
                        
        }
        
        return UINT_MAX;
        
    }
    
    void assignTask(const _task& task, const _stepPath& path){
        this->_currentPath = path;
        tasks.push_back(task);
        _currentTaskIndex = tasks.size() - 1;
    }
    
    void assignTaskSwap(const _task& task, const _stepPath& path);
    
    void unassignTaskSwap(const _task& task);
    void setPathSwap(const _stepPath& path);
          
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
    virtual void move(_token& token);
    virtual void receive(_token& token);
    
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

