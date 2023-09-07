/* 
 * File:   _selectRestEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 08:20
 */

#ifndef _SELECTRESTENDPOINTTOAGENTALGORITHM_H
#define _SELECTRESTENDPOINTTOAGENTALGORITHM_H

#include "_selectRestEndpointToAgentAlgorithm.h"
#include "_pathToAgentAlgorithm.h"
#include "_conflictEndpointTaskDeliveryAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"


class _selectRestEndpointToAgentAlgorithm{
public:
    
    _selectRestEndpointToAgentAlgorithm(
            const _endpointIndexerAlgorithm& endpointIndexerAlgorithm, 
            const _pathToAgentAlgorithm& pathToAgentAlgorithm,
            const _conflictEndpointTaskDeliveryAlgorithm& conflictEndpointTaskDeliveryAlgorithm) :
        endpointIndexerAlgorithm(endpointIndexerAlgorithm),
        pathToAgentAlgorithm(pathToAgentAlgorithm),
        conflictEndpointTaskDeliveryAlgorithm(conflictEndpointTaskDeliveryAlgorithm){ }

        
    _selectRestEndpointToAgentAlgorithm(
            const _selectRestEndpointToAgentAlgorithm& other) :
        endpointIndexerAlgorithm(other.endpointIndexerAlgorithm),
        pathToAgentAlgorithm(other.pathToAgentAlgorithm),
        conflictEndpointTaskDeliveryAlgorithm(other.conflictEndpointTaskDeliveryAlgorithm){ }

    virtual ~_selectRestEndpointToAgentAlgorithm(){}

    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _site& selectedNewSite, 
            _stepPath& selectedPath, 
            _task& conflitTask) const;
    
private:
    
    const _endpointIndexerAlgorithm& endpointIndexerAlgorithm;
    const _pathToAgentAlgorithm& pathToAgentAlgorithm;
    const _conflictEndpointTaskDeliveryAlgorithm& conflictEndpointTaskDeliveryAlgorithm;

};

#endif /* _SELECTRESTENDPOINTTOAGENTALGORITHM_H */

