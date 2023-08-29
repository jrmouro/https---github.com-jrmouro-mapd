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
    
    static _agent_state * getInstance(){
        
        if(_agent_occupied::_instance == nullptr)
            _agent_occupied::_instance = new _agent_occupied();
        
        return _agent_occupied::_instance;
        
    }
    
    virtual ~_agent_occupied(){}
        
    virtual std::string stateName()const{
        return "occupied";
    }
        
    virtual void onMoveUpdate(_system& system,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const;
       
    protected:
        _agent_occupied() : _agent_state() {}

    private:
        static _agent_state* _instance;
    
};

#endif /* _AGENT_OCCUPIED_H */

