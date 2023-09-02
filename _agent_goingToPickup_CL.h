/* 
 * File:   _agent_goingToPickup_CL.h
 * Author: ronaldo
 *
 * Created on 21 de agosto de 2023, 10:06
 */

#ifndef _AGENT_GOINGTOPICKUP_CL_H
#define _AGENT_GOINGTOPICKUP_CL_H

#include "_agent_goingToPickup.h"

class _agent_goingToPickup_CL : public _agent_goingToPickup {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToPickup_CL::_instance == nullptr)
            _agent_goingToPickup_CL::_instance = new _agent_goingToPickup_CL();
        
        return _agent_goingToPickup_CL::_instance;
        
    }
    
    virtual ~_agent_goingToPickup_CL(){}
            
    virtual std::string stateName()const{
        return "goingToPickup_CL";
    }
            
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    
    protected:        
        _agent_goingToPickup_CL() : _agent_goingToPickup() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOPICKUP_CL_H */

