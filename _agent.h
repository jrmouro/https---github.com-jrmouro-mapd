/* 
 * File:   _agent.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 09:30
 */

#ifndef _AGENT_H
#define _AGENT_H

#include "Identifiable.h"
#include "_stepPath.h"
#include "_task.h"
#include "_agent_state.h"
#include "Drawable.h"


class _token;
class _agent_state;
class _agent : public Identifiable<int>, public Drawable{
public:
        
    _agent(int _id, const _stepSite& currentSite);
    
    _agent(const _agent& other);

    virtual ~_agent();
    
    int id() const {
        return _id;
    }
    
    void changeState(_agent_state* _state) {
        if(this->_state != nullptr) delete this->_state;
        this->_state = _state;
    }
    
    
    
    void undesignTask(){
        _currentTaskIndex = -1;
    }
 
    const _stepSite& currentSite()const{
        
        return _currentPath.currentSite();
        
    }
    
    const _stepSite& goalSite() const{
        
        return _currentPath.goalSite();
        
    }
    
    const bool isParked()const{
        return _currentPath.size() == 1;
    }
    
    const bool isResting()const{
        
        return _currentTaskIndex == -1;
        
    }
    
    void designTask(const _task& task){
        tasks.push_back(task);
        _currentTaskIndex = tasks.size() - 1;
    }
    
    const bool isDesigned()const{
        
        return _currentTaskIndex > -1;
        
    }
    
    const bool isDelivering()const{
        
        if(_currentTaskIndex != -1){
            
            return isParked() && currentTask()->getDelivery().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    const bool isPickupping()const{
        
        if(_currentTaskIndex > -1){
            
            return currentTask()->getPickup().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    void stepping(){
        
        if (_currentPath.size() > 1) {

        _currentPath.pop();       

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
    
    virtual void move(_token& token);
    
    virtual void receive(_token& token);
    
    virtual bool selectNewEndpoint(_token& token, _site& selectNewSite);
          
    virtual bool selectNewTask(_token& token, _task& selectedTask);
    
    virtual bool isConflictingRestEndpoint(_token& token, _task& conflitTask) const;
    
    virtual void updateRestEndpointPath(_token& token, const _task& conflitTask);
    
    virtual void updateTaskPath(_token& token, const _task& task);
    
    virtual void updateTrivialPath(_token& token);
    
    virtual void draw(const Render& render) const;    
    
    friend std::ostream& operator<<(std::ostream& os, const _agent& obj) {
        os <<"agent id: "<< obj._id << std::endl;
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
        
        
protected:
    const int _id;
    _agent_state* _state;
    _stepPath _currentPath;    
    std::vector<_task> tasks;
    int _currentTaskIndex = -1;
    
    const _task* const currentTask() const {
        
        if(_currentTaskIndex > -1)
            return &tasks.at(_currentTaskIndex);
        return nullptr;
    }
};

#endif /* _AGENT_H */

