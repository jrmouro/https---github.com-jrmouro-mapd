/* 
 * File:   _agent_free.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 14:28
 */

#ifndef _AGENT_FREE_H
#define _AGENT_FREE_H

#include "_agent_state.h"


class _agent_free: public _agent_state {
public:
    _agent_free() : _agent_state() {}
    
    _agent_free(const _agent_free& other) : _agent_state() { }
    
    virtual ~_agent_free(){}
    
    virtual bool isFree()const{
        return true;
    }
    
    virtual AgentStateId stateId()const{
        return AgentStateId::free;
    }
    
    virtual std::string stateName()const{
        return "free";
    }
    
    virtual _agent_state* builder() const{
        return new _agent_free();
    }
    
    virtual void onUpdatePath(_token& token,  _agent* agent) const;
    virtual void onMoveUpdate(_token& token,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const; 
    

};

#endif /* _AGENT_FREE_H */

