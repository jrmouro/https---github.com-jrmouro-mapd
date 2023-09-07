/* 
 * File:   _updateChargingEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 19:05
 */

#ifndef _UPDATECHARGINGENDPOINTTOAGENTALGORITHM_H
#define _UPDATECHARGINGENDPOINTTOAGENTALGORITHM_H

#include "_selectChargingEndpointToAgentAlgorithm.h"

class _token;
class _updateChargingEndpointToAgentAlgorithm {
    
public:
    
    _updateChargingEndpointToAgentAlgorithm(
        const _endpointIndexerAlgorithm& endpointIndexerAlgorithm, 
        const _pathToAgentAlgorithm& pathToAgentAlgorithm):
            selectChargingEndpointToAgentAlgorithm(
                endpointIndexerAlgorithm, 
                pathToAgentAlgorithm){}
    
    _updateChargingEndpointToAgentAlgorithm(
        const _updateChargingEndpointToAgentAlgorithm& orig):
            selectChargingEndpointToAgentAlgorithm(
                orig.selectChargingEndpointToAgentAlgorithm){}
    
    virtual ~_updateChargingEndpointToAgentAlgorithm(){}
    
    virtual bool solve(
            _token& token, 
            _agent& agent) const;
    
private:
    
    _selectChargingEndpointToAgentAlgorithm selectChargingEndpointToAgentAlgorithm;

};

#endif /* _UPDATECHARGINGENDPOINTTOAGENTALGORITHM_H */

