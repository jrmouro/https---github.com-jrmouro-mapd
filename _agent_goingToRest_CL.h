/* 
 * File:   _agent_goingToRest_CL.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_GOINGTOREST_CL_H
#define _AGENT_GOINGTOREST_CL_H

#include "_agent_goingToRest.h"

class _agent_goingToRest_CL : public _agent_goingToRest {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToRest_CL::_instance == nullptr)
            _agent_goingToRest_CL::_instance = new _agent_goingToRest_CL();
        
        return _agent_goingToRest_CL::_instance;
        
    }
    
    virtual ~_agent_goingToRest_CL(){}
            
    virtual std::string stateName()const{
        return "goingToRest_CL";
    }
    
    virtual bool criticalEnergy() const {
        return true;
    }
        
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    
    protected:        
        _agent_goingToRest_CL() : _agent_goingToRest() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOREST_CL_H */

