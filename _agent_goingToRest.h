/* 
 * File:   _agent_goingToRest.h
 * Author: ronaldo
 *
 * Created on 22 de agosto de 2023, 02:55
 */

#ifndef _AGENT_GOINGTOREST_H
#define _AGENT_GOINGTOREST_H

#include "_agent_state.h"

class _agent_goingToRest : public _agent_state {
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToRest::_instance == nullptr)
            _agent_goingToRest::_instance = new _agent_goingToRest();
        
        return _agent_goingToRest::_instance;
        
    }
    
    virtual ~_agent_goingToRest(){}
             
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    virtual void onDraw(const Render&, const _agent&) const; 
    
    protected:        
        _agent_goingToRest() : _agent_state("goingToRest") {}
        _agent_goingToRest(const std::string& id) : _agent_state(id) {}
    
    private:
        static _agent_state* _instance;

};

#endif /* _AGENT_GOINGTOREST_H */

