/* 
 * File:   _agent_goingToCharging.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_GOINGTOCHARGING_H
#define _AGENT_GOINGTOCHARGING_H

#include "_agent_state.h"

class _agent_goingToCharging : public _agent_state {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToCharging::_instance == nullptr)
            _agent_goingToCharging::_instance = new _agent_goingToCharging();
        
        return _agent_goingToCharging::_instance;
        
    }
    
    virtual ~_agent_goingToCharging(){}
             
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    virtual void onDraw(const Render&, const _agent&) const; 
    
    protected:        
        _agent_goingToCharging() : _agent_state("goingToCharging") {}
        _agent_goingToCharging(const std::string& id) : _agent_state(id) {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOCHARGING_H */

