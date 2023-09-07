/* 
 * File:   _selectTrivialPathToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 16:48
 */

#ifndef _SELECTTRIVIALPATHTOAGENTALGORITHM_H
#define _SELECTTRIVIALPATHTOAGENTALGORITHM_H

#include "_agent.h"
#include "_stepPath.h"

class _selectTrivialPathToAgentAlgorithm {
public:
    
    virtual bool solve( 
            const _token& token,
            const _agent& agent, 
            _stepPath& trivialPath) const;

};

#endif /* _SELECTTRIVIALPATHTOAGENTALGORITHM_H */

