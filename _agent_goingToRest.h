/* 
 * File:   _agent_goingToRest.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_GOINGTOREST_H
#define _AGENT_GOINGTOREST_H

#include "_agent_free.h"

class _agent_goingToRest : public _agent_free {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToRest::_instance == nullptr)
            _agent_goingToRest::_instance = new _agent_goingToRest();
        
        return _agent_goingToRest::_instance;
        
    }
    
    virtual ~_agent_goingToRest(){}
            
    virtual std::string stateName()const{
        return "goingToRest (" + _agent_free::stateName() + ")" ;
    }
        
    virtual void onMoveUpdate(_system& system,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const; 
    
    protected:        
        _agent_goingToRest() : _agent_free() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOREST_H */

