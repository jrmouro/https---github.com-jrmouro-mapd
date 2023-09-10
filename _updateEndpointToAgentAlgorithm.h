/* 
 * File:   _updateRestEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 18:52
 */

#ifndef _UPDATERESTENDPOINTTOAGENTALGORITHM_H
#define _UPDATERESTENDPOINTTOAGENTALGORITHM_H

#include "_selectRestEndpointToAgentAlgorithm.h"

class _updateCloserRestEndpointToAgentAlgorithm {
    
public:
    
    _updateCloserRestEndpointToAgentAlgorithm( 
            const _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
            const _pathToAgentAlgorithm& pathToAgentAlgorithm,
            const _conflictEndpointTaskDeliveryAlgorithm& conflictEndpointTaskDeliveryAlgorithm):
                selectRestEndpointToAgentAlgorithm(
                    endpointIndexerAlgorithm,
                    pathToAgentAlgorithm,
                    conflictEndpointTaskDeliveryAlgorithm){}
    
    _updateCloserRestEndpointToAgentAlgorithm(const _updateCloserRestEndpointToAgentAlgorithm& orig):
    selectRestEndpointToAgentAlgorithm(orig.selectRestEndpointToAgentAlgorithm){}
    
    virtual ~_updateCloserRestEndpointToAgentAlgorithm(){}
    
    virtual bool solve(
            _token& token, 
            _agent& agent) const;
    
private:
    
    _selectRestEndpointToAgentAlgorithm selectRestEndpointToAgentAlgorithm;
    
};

#endif /* _UPDATECLOSERRESTENDPOINTTOAGENTALGORITHM_H */

