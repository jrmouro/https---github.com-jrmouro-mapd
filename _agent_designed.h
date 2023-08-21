/* 
 * File:   _agent_designed.h
 * Author: ronaldo
 *
 * Created on 21 de agosto de 2023, 10:06
 */

#ifndef _AGENT_DESIGNED_H
#define _AGENT_DESIGNED_H

#include "_agent_free.h"

class _agent_designed : public _agent_free {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_designed::_instance == nullptr)
            _agent_designed::_instance = new _agent_designed();
        
        return _agent_designed::_instance;
        
    }
    
    virtual ~_agent_designed(){}
            
    virtual std::string stateName()const{
        return "designed (" + _agent_free::stateName() + ")" ;
    }
        
    virtual void onUpdatePath(_system& system,  _agent* agent) const;
    virtual void onMoveUpdate(_system& system,  _agent* agent) const;
    virtual void onDraw(const Render& render, const _agent* const agent) const; 
    
    protected:        
        _agent_designed() : _agent_free() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_DESIGNED_H */

