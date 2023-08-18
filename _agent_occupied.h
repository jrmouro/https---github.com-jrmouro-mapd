/* 
 * File:   _agent_occupied.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 17:34
 */

#ifndef _AGENT_OCCUPIED_H
#define _AGENT_OCCUPIED_H

#include "_agent_state.h"

class _agent_occupied : public _agent_state{
    
public:
    
    _agent_occupied() :    _agent_state() {}
    _agent_occupied(const _agent_occupied& other) : _agent_state(other) {}

    virtual ~_agent_occupied(){}
    
    virtual AgentStateId stateId()const{
        return AgentStateId::occupied;
    }
    
    virtual std::string stateName()const{
        return "occupied";
    }
    
    virtual _agent_state* builder() const{
        return new _agent_occupied();
    }
    
    virtual void onUpdatePath(_token& token,  _agent* agent) const;
    virtual void onMoveUpdate(_token& token,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const;
    
    virtual bool isOccupied()const{
        return true;
    }
    
};

#endif /* _AGENT_OCCUPIED_H */

