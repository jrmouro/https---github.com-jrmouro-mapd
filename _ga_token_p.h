/* 
 * File:   _ga_token_p.h
 * Author: ronaldo
 *
 * Created on 10 de novembro de 2023, 07:24
 */

#ifndef _GA_TOKEN_P_H
#define _GA_TOKEN_P_H

#include "_ga_token.h"

class _ga_token_p : public _ga_token{
public:
    _ga_token_p(const _map&, const _stepMap&, const _agent_energy_system&);
    
    _ga_token_p(const _ga_token_p&);

    virtual ~_ga_token_p();
    
    virtual _ga_token* getClone()const;
    
    virtual std::string id() const;
    
    bool updateAgentTaskPath(int agentId, int newTaskId);
    
private:
    
    virtual bool updateAgentTaskPath_pendingTask(_ga_agent&, int, bool);
    virtual bool updateAgentTaskPath_going_to_rest_pickuping_to_pickuping(_ga_agent&, int);
    
    virtual bool updateAgentTaskPath_pickuping(_ga_agent&, int);

};

#endif /* _GA_TOKEN_P_H */

