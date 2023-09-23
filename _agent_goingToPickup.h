/* 
 * File:   _agent_goingToPickup.h
 * Author: ronaldo
 *
 * Created on 21 de agosto de 2023, 10:06
 */

#ifndef _AGENT_GOINGTOPICKUP_H
#define _AGENT_GOINGTOPICKUP_H

#include "_agent_state.h"

class _agent_goingToPickup : public _agent_state {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToPickup::_instance == nullptr)
            _agent_goingToPickup::_instance = new _agent_goingToPickup();
        
        return _agent_goingToPickup::_instance;
        
    }
    
    virtual ~_agent_goingToPickup(){}
                 
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    virtual void onDraw(const Render&, const _agent&) const;
    
    protected:        
        _agent_goingToPickup() : _agent_state("goingToPickup") {}
        _agent_goingToPickup(const std::string& id) : _agent_state(id) {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOPICKUP_H */

