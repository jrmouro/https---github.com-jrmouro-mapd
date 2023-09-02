/* 
 * File:   _agent_charging.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_CHARGING_H
#define _AGENT_CHARGING_H

#include "_agent_state.h"

class _agent_charging : public _agent_state {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_charging::_instance == nullptr)
            _agent_charging::_instance = new _agent_charging();
        
        return _agent_charging::_instance;
        
    }
    
    virtual ~_agent_charging(){}
            
    virtual std::string stateName()const{
        return "charging";
    }
        
    virtual void onUpdatePath(_token&, _agent&) const;
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    virtual void onDraw(const Render&, const _agent&) const; 
    
    protected:        
        _agent_charging() : _agent_state() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_PARKED_H */

