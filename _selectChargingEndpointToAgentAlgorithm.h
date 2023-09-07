/* 
 * File:   _selectChargingEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 08:20
 */

#ifndef _SELECTCHARGINGRESTENDPOINTTOAGENTALGORITHM_H
#define _SELECTCHARGINGRESTENDPOINTTOAGENTALGORITHM_H

#include "_endpointIndexerAlgorithm.h"
#include "_pathToAgentAlgorithm.h"


class _selectChargingEndpointToAgentAlgorithm{
public:
    
    _selectChargingEndpointToAgentAlgorithm(
            const _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
            const _pathToAgentAlgorithm& pathToAgentAlgorithm) :
        endpointIndexerAlgorithm(endpointIndexerAlgorithm),
        pathToAgentAlgorithm(pathToAgentAlgorithm){ }

        
    _selectChargingEndpointToAgentAlgorithm(
            const _selectChargingEndpointToAgentAlgorithm& other) :
        endpointIndexerAlgorithm(other.endpointIndexerAlgorithm),
        pathToAgentAlgorithm(other.pathToAgentAlgorithm){ }


    virtual ~_selectChargingEndpointToAgentAlgorithm(){}
    
    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _site& selectedNewSite, 
            _stepPath& selectedPath) const;
    
private:
    
    const _endpointIndexerAlgorithm& endpointIndexerAlgorithm;
    const _pathToAgentAlgorithm& pathToAgentAlgorithm;
    
};

#endif /* _SELECTCHARGINGRESTENDPOINTTOAGENTALGORITHM_H */

