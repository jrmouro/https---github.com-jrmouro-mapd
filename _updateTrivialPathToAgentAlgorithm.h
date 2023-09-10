/* 
 * File:   _updateTrivialPathToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 16:29
 */

#ifndef _UPDATETRIVIALPATHTOAGENTALGORITHM_H
#define _UPDATETRIVIALPATHTOAGENTALGORITHM_H

#include "_agent.h"
#include "_selectTrivialPathToAgentAlgorithm.h"

class _token;
class _updateTrivialPathToAgentAlgorithm {
    
public:
    
    _updateTrivialPathToAgentAlgorithm(){}
    _updateTrivialPathToAgentAlgorithm(const _updateTrivialPathToAgentAlgorithm& orig){}
    virtual ~_updateTrivialPathToAgentAlgorithm(){ }
    
    virtual bool solve(
            _token& token, 
            _agent& agent) const;
    
private:
    
    _selectTrivialPathToAgentAlgorithm selectTrivialPathToAgentAlgorithm;
    
};

#endif /* _UPDATETRIVIALPATHTOAGENTALGORITHM_H */

