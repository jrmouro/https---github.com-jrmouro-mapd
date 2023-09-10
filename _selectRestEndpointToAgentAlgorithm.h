/* 
 * File:   _selectRestEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 08:20
 */

#ifndef _SELECTRESTENDPOINTTOAGENTALGORITHM_H
#define _SELECTRESTENDPOINTTOAGENTALGORITHM_H

#include "_selectRestEndpointToAgentAlgorithm.h"
#include "_selectEndpointToAgentAlgorithm.h"
#include "_pathToAgentAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"


class _selectRestEndpointToAgentAlgorithm : public _selectEndpointToAgentAlgorithm{
public:
    
    _selectRestEndpointToAgentAlgorithm(
            _endpointIndexerAlgorithm& endpointIndexerAlgorithm, 
            const _pathToAgentAlgorithm& pathToAgentAlgorithm) :
        _selectEndpointToAgentAlgorithm(endpointIndexerAlgorithm),
        pathToAgentAlgorithm(pathToAgentAlgorithm){ }

        
    _selectRestEndpointToAgentAlgorithm(
            const _selectRestEndpointToAgentAlgorithm& other) :
        _selectEndpointToAgentAlgorithm(other),
        pathToAgentAlgorithm(other.pathToAgentAlgorithm){ }

    virtual ~_selectRestEndpointToAgentAlgorithm(){}

    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _site& selectedNewSite, 
            _stepPath& selectedPath) const;
        
protected:
    
    const _pathToAgentAlgorithm& pathToAgentAlgorithm;

};

#endif /* _SELECTRESTENDPOINTTOAGENTALGORITHM_H */

