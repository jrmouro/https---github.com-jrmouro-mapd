/* 
 * File:   _selectChargingTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 19:03
 */

#ifndef _SELECTCHARGINGTASKTOAGENTALGORITHM_H
#define _SELECTCHARGINGTASKTOAGENTALGORITHM_H

#include "_selectTaskToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"

class _selectChargingTaskToAgentAlgorithm : public _selectTaskToAgentAlgorithm{
public:

    _selectChargingTaskToAgentAlgorithm(
            const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
            _taskIndexerAlgorithm& taskIndexerAlgorithm,
            _endpointIndexerAlgorithm& endpointIndexerAlgorithm) :
    
    _selectTaskToAgentAlgorithm(taskPathToAgentAlgorithm, taskIndexerAlgorithm),
    endpointIndexerAlgorithm(endpointIndexerAlgorithm) { }

    _selectChargingTaskToAgentAlgorithm(const _selectChargingTaskToAgentAlgorithm& other) :
        _selectTaskToAgentAlgorithm(other),
        endpointIndexerAlgorithm(other.endpointIndexerAlgorithm) { }

    virtual ~_selectChargingTaskToAgentAlgorithm() { }

    virtual bool solve(
            const _token& token,
            const _agent& agent,
            _task& task,
            _stepPath& path) const;
    
    
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm);

private:

    _endpointIndexerAlgorithm& endpointIndexerAlgorithm;
    
};

#endif /* _SELECTCHARGINGTASKTOAGENTALGORITHM_H */

