/* 
 * File:   Agent.h
 * Author: ronaldo
 *
 * Created on 2 de agosto de 2023, 10:53
 */

#ifndef AGENT_H
#define AGENT_H

#include "Site.h"
#include "BinaryPath.h"


class Agent : public Identifiable<int>{
    
public:
    
    enum State{ free, occupied };
            
    Agent(int _id, const Site& current) :  _id(_id), path(current) { }
    Agent(const Agent& other) : _id(other._id), state(other.state), path(other.path) { }
    
    virtual int id() const{
        return this->_id;
    }
    
    State getState() const {
        return state;
    }

    void setState(State state) {
        this->state = state;
    }
    
    virtual ~Agent(){}
    
    virtual void receive(unsigned step, Token& token){
        
                
    }
    
    virtual void move(unsigned step, Token& token){
                
        
    }
    
private:
    int _id;
    State state = free;
    BinaryPath path;
};

#endif /* AGENT_H */

