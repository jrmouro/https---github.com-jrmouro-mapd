/* 
 * File:   _updateTrivialPathToAgent.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 16:29
 */

#ifndef _UPDATETRIVIALPATHTOAGENT_H
#define _UPDATETRIVIALPATHTOAGENT_H

#include "_agent.h"
#include "_selectTrivialPathToAgentAlgorithm.h"

class _token;
class _updateTrivialPathToAgent {
    
public:
    
    _updateTrivialPathToAgent(){}
    _updateTrivialPathToAgent(const _updateTrivialPathToAgent& orig){}
    virtual ~_updateTrivialPathToAgent(){ }
    
    virtual bool solve(
            _token& token, 
            _agent& agent) const;
    
private:
    
    _selectTrivialPathToAgentAlgorithm selectTrivialPathToAgentAlgorithm;
    
};

#endif /* _UPDATETRIVIALPATHTOAGENT_H */

