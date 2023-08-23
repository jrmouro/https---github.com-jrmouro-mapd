/* 
 * File:   _agent_state.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 10:39
 */

#ifndef _AGENT_STATE_H
#define _AGENT_STATE_H

#include "_task.h"
#include "_stepMap.h"

class _system;
class Render;
class _agent;

class _agent_state {
    
public:
        
    _agent_state(){}    
    _agent_state(const _agent_state& other) {}   
    
    virtual ~_agent_state(){} 
    
    virtual std::string stateName()const = 0;
      
    virtual void changeState(_agent* agent, _agent_state* state) const;
        
    virtual void onUpdatePath(_system& system,  _agent* agent) const;
    virtual void onMoveUpdate(_system& system,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const;
                
};

#endif /* _AGENT_STATE_H */

