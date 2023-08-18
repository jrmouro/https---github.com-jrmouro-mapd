/* 
 * File:   _agent_state.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 10:39
 */

#ifndef _AGENT_STATE_H
#define _AGENT_STATE_H

#include "_task.h"

class _token;
class Render;
class _agent;

class _agent_state {
    
public:
    
    enum AgentStateId {
        free,
        occupied,
        charging,
        discharged
    };
        
    _agent_state(){}    
    _agent_state(const _agent_state& other) {}   
    
    virtual ~_agent_state(){} 
    
    virtual bool isFree()const{
        return false;
    }

    virtual bool isOccupied()const{
        return false;
    }

    virtual bool isCharging()const{
        return false;
    }

    virtual bool isDischarged()const{
        return false;
    }
    
    virtual AgentStateId stateId()const = 0;
    virtual std::string stateName()const = 0;
      
    virtual _agent_state* builder() const = 0;
    
    virtual void onUpdatePath(_token& token,  _agent* agent) const;
    virtual void onMoveUpdate(_token& token,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const;
        
};

#endif /* _AGENT_STATE_H */

