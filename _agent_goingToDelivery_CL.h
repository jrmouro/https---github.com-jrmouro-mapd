/* 
 * File:   _agent_goingToDelivery_CL.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 17:34
 */

#ifndef _AGENT_GOINGTODELIVERY_CL_H
#define _AGENT_GOINGTODELIVERY_CL_H

#include "_agent_goingToDelivery.h"

class _agent_goingToDelivery_CL : public _agent_goingToDelivery{
    
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToDelivery_CL::_instance == nullptr)
            _agent_goingToDelivery_CL::_instance = new _agent_goingToDelivery_CL();
        
        return _agent_goingToDelivery_CL::_instance;
        
    }
    
    virtual ~_agent_goingToDelivery_CL(){}
        
    virtual std::string stateName()const{
        return "goingToDelivery_CL";
    }
    
    virtual bool free() const{
        return false;
    }
    
    virtual bool criticalEnergy() const {
        return true;
    }
        
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
       
    protected:
        _agent_goingToDelivery_CL() : _agent_goingToDelivery() {}

    private:
        static _agent_state* _instance;
    
};

#endif /* _AGENT_GOINGTODELIVERY_CL_H */

