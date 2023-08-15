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
#include "Task.h"
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
    
    void setState(_agent_state* _state) {
        if(this->_state != nullptr) delete this->_state;
        this->_state = _state;
    }
    
    const Task* currentTask() const {
        return _currentTask;
    }
    
    void undesignTask(){
        if(_currentTask != nullptr) delete _currentTask;
        _currentTask = nullptr;
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
        
        return _currentTask == nullptr;
        
    }
    
    const bool isDesigned()const{
        
        return _currentTask != nullptr;
        
    }
    
    const bool isDelivering()const{
        
        if(_currentTask != nullptr){
            
            return isParked() && _currentTask->getDelivery().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    const bool isPickupping()const{
        
        if(_currentTask != nullptr){
            
            return _currentTask->getPickup().match(_currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    void stepping(){
        
        if (_currentPath.size() > 1) {

        _currentPath.pop();       

        } else {

            try {
                std::ostringstream stream;
                stream << "invalid path size";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }
        
    }
    
    virtual void move(_token& token);
    
    virtual void receive(_token& token);
    
    virtual _site* selectNewEndpoint(_token& token);
          
    virtual bool selectNewTask(_token& token);
    
    virtual bool isConflictingRestEndpoint(_token& token, Task* const task) const;
    
    virtual void updateRestEndpointPath(_token& token, const Task& conflitTask);
    
    virtual void updateTaskPath(_token& token);
    
    virtual void updateTrivialPath(_token& token);
    
    virtual void draw(const Render& render) const{
        this->_state->onDraw(render);
    }
        
protected:
    const int _id;
    _agent_state* _state;
    _stepPath _currentPath;    
    Task* _currentTask = nullptr;
};

#endif /* _AGENT_H */

