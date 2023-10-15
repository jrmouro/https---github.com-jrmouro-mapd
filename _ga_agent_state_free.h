/* 
 * File:   _ga_agent_state_free.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 07:42
 */

#ifndef _GA_AGENT_STATE_FREE_H
#define _GA_AGENT_STATE_FREE_H

#include "_ga_agent_state.h"

class _ga_agent_state_free : public _ga_agent_state{
public:
    
    static _ga_agent_state* getInstance();
    
    virtual ~_ga_agent_state_free();
    virtual std::string id() const;
    
    virtual void onStepping(const _map&, _ga_agent&) const;
    
protected:
    _ga_agent_state_free();
    
private:
    static _ga_agent_state* instance;
};

#endif /* _GA_AGENT_STATE_FREE_H */

