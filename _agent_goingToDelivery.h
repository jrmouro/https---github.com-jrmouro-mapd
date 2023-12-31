/* 
 * File:   _agent_goingToDelivery.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 17:34
 */

#ifndef _AGENT_GOINGTODELIVERY_H
#define _AGENT_GOINGTODELIVERY_H

#include "_agent_state.h"

class _agent_goingToDelivery : public _agent_state{
    
public:
    
    static _agent_state * getInstance(){
        
        if(_agent_goingToDelivery::_instance == nullptr)
            _agent_goingToDelivery::_instance = new _agent_goingToDelivery();
        
        return _agent_goingToDelivery::_instance;
        
    }
    
    virtual ~_agent_goingToDelivery(){}
          
    virtual bool free() const{
        return false;
    }
        
    virtual void onAfterStepping(_token&, _agent&) const;
    virtual void onEnergyExpend(_token&, _agent&) const;
    virtual void onDraw(const Render&, const _agent&) const;
       
    protected:
        _agent_goingToDelivery() : _agent_state("goingToDelivery") {}
        _agent_goingToDelivery(const std::string& id) : _agent_state(id) {}

    private:
        static _agent_state* _instance;
    
};

#endif /* _AGENT_GOINGTODELIVERY_H */

