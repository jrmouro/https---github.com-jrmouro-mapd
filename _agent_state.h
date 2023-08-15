/* 
 * File:   _agent_state.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 10:39
 */

#ifndef _AGENT_STATE_H
#define _AGENT_STATE_H

#include "Task.h"

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
        
    _agent_state(_agent* agent) : agent(agent) {}    
    _agent_state(const _agent_state& other) : agent(other.agent) {}    
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
      
    virtual _agent_state* builder(AgentStateId stateId, _agent* agent) const;
    virtual void onUpdatePath(_token& token) const;
    virtual void onMoveUpdate(_token& token) const;
    virtual void onDraw(const Render& render) const;
        
protected:
    _agent* agent;
};

#endif /* _AGENT_STATE_H */

