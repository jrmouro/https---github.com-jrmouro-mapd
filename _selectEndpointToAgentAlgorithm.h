/* 
 * File:   _selectEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 10 de setembro de 2023, 05:13
 */

#ifndef _SELECTENDPOINTTOAGENTALGORITHM_H
#define _SELECTENDPOINTTOAGENTALGORITHM_H

#include "_endpointIndexerAlgorithm.h"
#include "_token.h"
#include "_agent.h"
#include "_site.h"
#include "_stepPath.h"

class _selectEndpointToAgentAlgorithm {
public:
    
    _selectEndpointToAgentAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm) :
    endpointIndexerAlgorithm(endpointIndexerAlgorithm) { }

    _selectEndpointToAgentAlgorithm(const _selectEndpointToAgentAlgorithm& other) :
    endpointIndexerAlgorithm(other.endpointIndexerAlgorithm) { }
    
    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _site& selectedNewSite, 
            _stepPath& selectedPath) const = 0;
    
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm) {
        this->endpointIndexerAlgorithm = endpointIndexerAlgorithm;
    }
    
protected:
    
    _endpointIndexerAlgorithm& endpointIndexerAlgorithm;
    
};

#endif /* _SELECTENDPOINTTOAGENTALGORITHM_H */

