/* 
 * File:   _selectChargingEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 08:20
 */

#ifndef _SELECTCHARGINGRESTENDPOINTTOAGENTALGORITHM_H
#define _SELECTCHARGINGRESTENDPOINTTOAGENTALGORITHM_H

#include "_endpointIndexerAlgorithm.h"
#include "_selectRestEndpointToAgentAlgorithm.h"
#include "_pathToAgentAlgorithm.h"


class _selectChargingEndpointToAgentAlgorithm  : public _selectRestEndpointToAgentAlgorithm{
public:
    
    _selectChargingEndpointToAgentAlgorithm(
            _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
            const _pathToAgentAlgorithm& pathToAgentAlgorithm) :
        _selectRestEndpointToAgentAlgorithm(endpointIndexerAlgorithm, pathToAgentAlgorithm){ }

        
    _selectChargingEndpointToAgentAlgorithm(
            const _selectChargingEndpointToAgentAlgorithm& other) :
            _selectRestEndpointToAgentAlgorithm(other){ }


    virtual ~_selectChargingEndpointToAgentAlgorithm(){}
    
    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _site& selectedNewSite, 
            _stepPath& selectedPath) const;
            
};

#endif /* _SELECTCHARGINGRESTENDPOINTTOAGENTALGORITHM_H */

