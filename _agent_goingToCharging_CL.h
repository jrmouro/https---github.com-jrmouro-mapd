/* 
 * File:   _agent_goingToCharging_CL.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_GOINGTOCHARGING_CL_H
#define _AGENT_GOINGTOCHARGING_CL_H

#include "_agent_goingToCharging.h"

class _agent_goingToCharging_CL : public _agent_goingToCharging {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToCharging_CL::_instance == nullptr)
            _agent_goingToCharging_CL::_instance = new _agent_goingToCharging_CL();
        
        return _agent_goingToCharging_CL::_instance;
        
    }
    
    virtual ~_agent_goingToCharging_CL(){}
            
    virtual std::string stateName()const{
        return "goingToCharging_CL";
    }
        
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    
    protected:        
        _agent_goingToCharging_CL() : _agent_goingToCharging() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOCHARGING_CL_H */

