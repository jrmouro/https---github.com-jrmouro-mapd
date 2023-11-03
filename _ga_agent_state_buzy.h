/* 
 * File:   _ga_agent_state_buzy.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 08:20
 */

#ifndef _GA_AGENT_STATE_BUZY_H
#define _GA_AGENT_STATE_BUZY_H

#include "_ga_agent_state.h"

class _ga_agent_state_buzy : public _ga_agent_state{
public:
    static _ga_agent_state* getInstance();
    
    virtual ~_ga_agent_state_buzy();
    virtual int id() const;
    
    virtual void onDraw(const Render&, const _ga_agent&) const;
    virtual void onStepping(const _map&, _ga_agent&) const;
    
protected:
    _ga_agent_state_buzy();
    
private:
    static _ga_agent_state* instance;

};

#endif /* _GA_AGENT_STATE_BUZY_H */

