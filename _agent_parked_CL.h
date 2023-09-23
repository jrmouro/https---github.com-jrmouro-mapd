/* 
 * File:   _agent_parked_CL.h
 * Author: ronaldo
 *
 * Created on 25 de agosto de 2023, 11:41
 */

#ifndef _AGENT_PARKED_CL_H
#define _AGENT_PARKED_CL_H

#include "_agent_parked.h"

class _agent_parked_CL : public _agent_parked {
    
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_parked_CL::_instance == nullptr)
            _agent_parked_CL::_instance = new _agent_parked_CL();
        
        return _agent_parked_CL::_instance;
        
    }
    
    virtual ~_agent_parked_CL(){}
    
    virtual bool criticalEnergy() const {
        return true;
    }

    virtual void onUpdatePath(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    virtual void onAfterStepping(_token&,  _agent&) const;
    
    protected:        
        _agent_parked_CL() : _agent_parked("parked_CL") {}
    
    private:
        static _agent_state* _instance;
        

};

#endif /* _AGENT_PARKED_CL_H */

