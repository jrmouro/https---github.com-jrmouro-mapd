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
    
    static _agent_state * getInstance(){
        
        if(_agent_free::_instance == nullptr)
            _agent_free::_instance = new _agent_free();
        
        return _agent_free::_instance;
        
    }
    
    virtual ~_agent_free(){}
            
    virtual std::string stateName()const{
        return "free";
    }
        
    virtual void onUpdatePath(_system& system,  _agent* agent) const;
    virtual void onMoveUpdate(_system& system,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const; 
    
    protected:        
        _agent_free() : _agent_state() {}
    
    private:
        static _agent_state* _instance;
        

};

#endif /* _AGENT_FREE_H */

