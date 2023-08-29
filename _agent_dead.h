/* 
 * File:   _agent_dead.h
 * Author: ronaldo
 *
 * Created on 25 de agosto de 2023, 11:28
 */

#ifndef _AGENT_DEAD_H
#define _AGENT_DEAD_H

#include "_agent_state.h"

class _agent_dead: public _agent_state {
    
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_dead::_instance == nullptr)
            _agent_dead::_instance = new _agent_dead();
        
        return _agent_dead::_instance;
        
    }
    
    virtual ~_agent_dead(){}
            
    virtual std::string stateName()const{
        return "dead";
    }

    virtual void onDraw(const Render& render, const _agent* const agent) const; 
    virtual void onUpdatePath(_system& system,  _agent* agent) const;
    virtual void onAfterStepping(_system& system,  _agent* agent) const;
    
    protected:        
        _agent_dead() : _agent_state() {}
    
    private:
        static _agent_state* _instance;
        

};

#endif /* _AGENT_DEAD_H */

