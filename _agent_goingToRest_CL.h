/* 
 * File:   _agent_goingToRest_CL.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_GOINGTOREST_CL_H
#define _AGENT_GOINGTOREST_CL_H

#include "_agent_state.h"

class _agent_goingToRest_CL : public _agent_state {
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
        
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    virtual void onDraw(const Render&, const _agent&) const; 
    
    protected:        
        _agent_goingToRest_CL() : _agent_state() {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOREST_CL_H */

