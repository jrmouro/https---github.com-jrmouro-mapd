/* 
 * File:   _agent_charging_CL.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_CHARGING_CL_H
#define _AGENT_CHARGING_CL_H

#include "_agent_charging.h"

class _agent_charging_CL : public _agent_charging {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_charging_CL::_instance == nullptr)
            _agent_charging_CL::_instance = new _agent_charging_CL();
        
        return _agent_charging_CL::_instance;
        
    }
    
    virtual ~_agent_charging_CL(){}
        
    virtual bool criticalEnergy() const {
        return true;
    }
        
    bool charging() const  { 
        return true; 
    }
        
    virtual void onUpdatePath(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const; 
    virtual void onAfterStepping(_token&, _agent&) const;
    
    protected:        
        _agent_charging_CL() : _agent_charging("charging_CL") {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_CHARGING_CL_H */

