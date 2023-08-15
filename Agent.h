/* 
 * File:   Agent.h
 * Author: ronaldo
 *
 * Created on 2 de agosto de 2023, 10:53
 */

#ifndef AGENT_H
#define AGENT_H

#include "Site.h"
#include "Task.h"
#include "ManhattanAlgorithm.h"
#include "ReportTask.h"

class Token;
class Agent : public Identifiable<int>, public Drawable{
    
public:
    
    enum State{ free, occupied };
            
    Agent(int _id, const _stepSite& current) :  _id(_id), currentPath(current) { }
    Agent(const Agent& other) : _id(other._id), state(other.state), currentPath(other.currentPath) { }
    
    virtual int id() const{
        return this->_id;
    }
    
    State getState() const {
        return state;
    }

    void setState(State state) {
        this->state = state;
    }
    
    virtual ~Agent(){
    
        if(this->currentTask != nullptr) delete this->currentTask;
    
    }
    
    void receive(Token& token);
    
    void move(Token& token);
    
    const _stepSite& currentSite()const{
        
        return currentPath.currentSite();
        
    }
    
    const _stepSite& goalSite() const{
        
        return currentPath.goalSite();
        
    }
    
    bool isParked()const{
        return currentPath.size() == 1;
    }
    
    bool isResting()const{
        
        return currentTask == nullptr;
        
    }
    
    bool isDesignated()const{
        
        return currentTask != nullptr;
        
    }
    
    bool isDelivering()const{
        
        if(currentTask != nullptr){
            
            return isParked() && currentTask->getDelivery().match(currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    bool isPickupping()const{
        
        if(currentTask != nullptr){
            
            return currentTask->getPickup().match(currentPath.currentSite());
            
        }
        
        return false;
        
    }
    
    virtual void draw(const Render& render) const;
    
protected:
    
    virtual _site* selectNewEndpoint(Token& token);
      
//    virtual Task* selectNewTask(Token& token) const;
    
    virtual bool selectNewTask(Token& token);
    
    virtual bool isConflictingRestEndpoint(Token& token) const;
    
    virtual void updateRestEndpointPath(Token& token);
    
//    virtual bool updateRestEndpointPath(Token& token);
    
    virtual void updateTaskPath(Token& token);
    
//    virtual bool updateTaskPath(Token& token);
    
    virtual void updateTrivialPath(Token& token);
    
//    virtual bool updateTrivialPath(Token& token);
       
    virtual void updatePath(Token& token);
    
    virtual void moveUpdate(Token& token);
    
private:
    int _id;
    State state = free;
    _stepPath currentPath;
    Task* currentTask = nullptr;
    
};

#endif /* AGENT_H */

