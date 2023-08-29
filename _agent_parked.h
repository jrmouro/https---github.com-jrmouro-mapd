/* 
 * File:   _agent_parked.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_PARKED_H
#define _AGENT_PARKED_H

#include "_agent_state.h"

class _agent_parked : public _agent_state {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_parked::_instance == nullptr)
            _agent_parked::_instance = new _agent_parked();
        
        return _agent_parked::_instance;
        
    }
    
    virtual ~_agent_parked(){}
            
    virtual std::string stateName()const{
        return "parked";
    }
    
    virtual void onUpdatePath(_token&, _agent&) const;
    virtual void onBeforeStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    
    protected:        
        _agent_parked() : _agent_state() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_PARKED_H */

