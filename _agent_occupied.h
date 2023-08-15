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
    
    _agent_occupied(_agent* agent) :    _agent_state(agent) {}
    _agent_occupied(const _agent_occupied& other) : _agent_state(other) {}

    virtual ~_agent_occupied(){}
    
    virtual AgentStateId stateId()const{
        return AgentStateId::occupied;
    }
    
    
//    virtual void onUpdatePath(_token& token) const;
    virtual void onMoveUpdate(_token& token) const;
    virtual void onDraw(const Render& render) const;  
    
    virtual bool isOccupied()const{
        return true;
    }

};

#endif /* _AGENT_OCCUPIED_H */

