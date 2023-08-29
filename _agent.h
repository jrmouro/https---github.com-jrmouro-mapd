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
    
    virtual _agent* instance()const{
        return new _agent(*this);
    }
    
    bool isAbleToFulfillTaskPath(const _map& map, const _task& task, const _stepPath& taskPath, AES& aes)const{
        int need = this->energy_system.appraiseTaskPath(map, task, taskPath, aes);
        return need <= this->energy_system.currentLevel();
    }
    
    bool isAbleToFulfillNoCarryngPath(const _map& map, const _stepPath& path, AES& aes)const{
        int need = this->energy_system.appraiseNoCarryngPath(map, path, aes);
        return need <= this->energy_system.currentLevel();
    }
     
    
    const _stepPath& currentPath()const{
        return _currentPath;
    }
 
    const _stepSite& currentSite()const{
        
        return _currentPath.currentSite();
        
    }
    
    const _stepSite& futureSite()const{
        
        return _currentPath.futureSite();
        
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
        
        return _currentTaskIndex > -1 && !_charging_task;
        
    }
    
    const bool isChargingTaskAssigned()const{
        
        return _currentTaskIndex > -1  && _charging_task;
        
    }
    
    const bool wasSendToRest()const{
        
        return !isAssigned() && _currentPath.size() > 2 && !_charging_path;
        
    }
    
    const bool wasSendToCharging()const{
        
        return !isAssigned() && _currentPath.size() > 2 && energy_system.isAtCriticalLevel();
        
    }
    
//    const bool isParkedCharging()const{
//        
//        return _currentPath.size() == 1 && !energy_system.isMaximumLevel();
//        
//    }
    
    const bool isDelivering()const{
        
        if(_currentTaskIndex > -1){
            
            return isInFinishedPath() && currentTask()->getDelivery().match(_currentPath.goalSite());
            
        }
        
        return false;
        
    }
    
    const bool isChargingDelivering()const{
        
        if(_currentTaskIndex > -1 && !currentTask()->getDelivery().match(_currentPath.goalSite())){
            
            return !isInFinishedPath() &&  currentTask()->getDelivery().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    const bool isPickupping()const{
        
        if(_currentTaskIndex > -1){
            
            return currentTask()->getPickup().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    const bool isChargingPickupping()const{
        
        if(_currentTaskIndex > -1 && !currentTask()->getDelivery().match(_currentPath.goalSite())){
            
            return currentTask()->getPickup().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
       
    virtual void draw(const Render& render) const;    
    
    friend std::ostream& operator<<(std::ostream& os, const _agent& obj) {
        os <<"agent id: "<< obj._id << std::endl;
        os << "energy " << obj.energy_system << std::endl;
        os <<"agent path: "<< obj._currentPath << std::endl;
        if(obj._currentTaskIndex > -1)
            os <<"agent current task: "<< obj.getCurrentTask() << std::endl;
        os <<"agent state: "<< obj._state->stateName() << std::endl;
        return os;
    }
    
    _task getCurrentTask() const{
        
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
        
        return _task();
        
    }
    
    int amountEnergy() const {
        return energy_system.currentLevel();
    }
    
    bool isAtEnergyChargingLevel()const{
        return energy_system.isAtChargingLevel();
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
    
       
        
protected:
    const int _id;
    _agent_state* _state;
    _stepPath _currentPath;    
    std::vector<_task> tasks;
    int _currentTaskIndex = -1;
    _agent_energy_system energy_system;
    _stepSite _previousSite;
    bool _charging_task = false, _charging_path = true;
    
    const _task* const currentTask() const {
        
        if(_currentTaskIndex > -1)
            return &tasks.at(_currentTaskIndex);
        return nullptr;
    }
    
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
    
    void expendEnergy(AER aer) {
        energy_system.expend(aer);       
    }
    
    void expendCarryngStepping(bool isChargingSite, AES& aes){
        energy_system.expendCarryngStepping(isChargingSite, _currentPath.currentSite(), _currentPath.futureSite(), aes);
    }
    
    void expendNoCarryngStepping(bool isChargingSite, AES& aes){
        energy_system.expendNoCarryngStepping(isChargingSite, _currentPath.currentSite(), _currentPath.futureSite(), aes);
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
    
private: // events    
    
    
    friend class _token;
    
    void pickup(_token& token, AES aes){
        
        switch(aes){
            case AES::critical:
                _state->onPickupInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onPickupInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onPickupInEnergyDeadLevel(token, *this);
                break;
        }

    }
    
    void delivery(_token& token, AES aes){
        
        switch(aes){
            case AES::critical:
                _state->onDeliveryInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onDeliveryInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onDeliveryInEnergyDeadLevel(token, *this);
                break;
        }

    }
    
    void chargingPickup(_token& token, AES aes){
        
        switch(aes){
            case AES::critical:
                _state->onChargingPickupInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onChargingPickupInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onChargingPickupInEnergyDeadLevel(token, *this);
                break;
        }

    }
    
    void chargingDelivery(_token& token, AES aes){
        
        switch(aes){
            case AES::critical:
                _state->onChargingDeliveryInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onChargingDeliveryInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onChargingDeliveryInEnergyDeadLevel(token, *this);
                break;
        }

    }
    
    void chargingTaskEndpoint(_token& token, AES aes){
        
        switch(aes){
            case AES::critical:
                _state->onChargingTaskEndpointInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onChargingTaskEndpointInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onChargingTaskEndpointInEnergyDeadLevel(token, *this);
                break;
        }

    }
    
    
    void assignTask(_token& token, const _task& task, const _stepPath& path, AES aes){
        this->_currentPath = path;
        tasks.push_back(task);
        _currentTaskIndex = tasks.size() - 1;
        _charging_task = false; 
        
        switch(aes){
            case AES::critical:
                _state->onAssignedTaskInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onAssignedTaskInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onAssignedTaskInEnergyDeadLevel(token, *this);
                break;
        }
        
    }
    
    void assignChargingTask(_token& token, const _task& task, const _stepPath& path, AES aes){
        this->_currentPath = path;
        tasks.push_back(task);
        _currentTaskIndex = tasks.size() - 1;
        _charging_task = true;   
        
        switch(aes){
            case AES::critical:
                _state->onAssignedChargingTaskInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onAssignedChargingTaskInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onAssignedChargingTaskInEnergyDeadLevel(token, *this);
                break;
        }
    }
    
    void assignRestPath(_token& token, const _stepPath& path, AES aes){
        this->_currentPath = path;
        _charging_task = false; 
        _charging_path = false;
        
        switch(aes){
            case AES::critical:
                _state->onAssignedRestPathInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onAssignedRestPathInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onAssignedRestPathInEnergyDeadLevel(token, *this);
                break;
        }
    }
    
    void assignChargingPath(_token& token, const _stepPath& path, AES aes){
        this->_currentPath = path; 
        _charging_task = false; 
        _charging_path = true;
        
        switch(aes){
            case AES::critical:
                _state->onAssignedChargingPathInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onAssignedChargingPathInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onAssignedChargingPathInEnergyDeadLevel(token, *this);
                break;
        }
    }
    
    void assignTrivialPath(_token& token, const _stepPath& path, AES aes){
        
        this->_currentPath = path;        
        _charging_task = false; 
        _charging_path = false;  
        
        switch(aes){
            case AES::critical:
                _state->onAssignedTrivialPathInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onAssignedTrivialPathInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onAssignedTrivialPathInEnergyDeadLevel(token, *this);
                break;
        }
    }
    
    void assignChargingTrivialPath(_token& token, const _stepPath& path, AES aes){
        
        this->_currentPath = path;
        
        _charging_task = false; 
        _charging_path = true; 
        
        switch(aes){
            case AES::critical:
                _state->onAssignedChargingTrivialPathInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onAssignedChargingTrivialPathInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onAssignedChargingTrivialPathInEnergyDeadLevel(token, *this);
                break;
        }
    }
                
    void unassignTask(_token& token, AES aes){
        _currentTaskIndex = -1;
        _charging_task = false; 
        _charging_path = false;
        
        switch(aes){
            case AES::critical:
                _state->onUnassignedTaskInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onUnassignedTaskInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onUnassignedTaskInEnergyDeadLevel(token, *this);
                break;
        }
        
    }
    
    void unassignChargingTask(_token& token, AES aes){
        _currentTaskIndex = -1;
        _charging_task = true; 
        _charging_path = true;
        
        switch(aes){
            case AES::critical:
                _state->onUnassignedChargingTaskInEnergyCriticalLevel(token, *this);
                break;
            case AES::normal:
                _state->onUnassignedChargingTaskInEnergyNormalLevel(token, *this);
                break;
            case AES::dead:
                _state->onUnassignedChargingTaskInEnergyDeadLevel(token, *this);
                break;
        }
        
    }
        
};

#endif /* _AGENT_H */

