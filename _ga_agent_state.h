/* 
 * File:   _ga_agent_state.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 07:34
 */

#ifndef _GA_AGENT_STATE_H
#define _GA_AGENT_STATE_H

#include "Identifiable.h"
#include "Render.h"
#include "_map.h"
#include <string>

class _ga_agent;

class _ga_agent_state : public Identifiable<std::string>{
public:
    void changeState(_ga_agent&, _ga_agent_state*) const;
    virtual void onDraw(const Render&, const _ga_agent&) const;
    virtual void onStepping(const _map&, _ga_agent&)const = 0;
};

#endif /* _GA_AGENT_STATE_H */

