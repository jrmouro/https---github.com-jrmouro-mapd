/* 
 * File:   _selectChargingTaskToAgentThresholdAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 19:03
 */

#ifndef _SELECTCHARGINGTASKTOAGENTTHRESHOLDALGORITHM_H
#define _SELECTCHARGINGTASKTOAGENTTHRESHOLDALGORITHM_H

#include "_selectTaskToAgentThresholdAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"

class _selectChargingTaskToAgentThresholdAlgorithm : public _selectTaskToAgentThresholdAlgorithm{
public:

    _selectChargingTaskToAgentThresholdAlgorithm(
            const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
            _taskIndexerAlgorithm& taskIndexerAlgorithm,
            _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
            float pickup_threshold,
            float delivery_threshold) :
    
    _selectTaskToAgentThresholdAlgorithm(taskPathToAgentAlgorithm, taskIndexerAlgorithm, pickup_threshold, delivery_threshold),
    endpointIndexerAlgorithm(endpointIndexerAlgorithm) { }

    _selectChargingTaskToAgentThresholdAlgorithm(const _selectChargingTaskToAgentThresholdAlgorithm& other) :
        _selectTaskToAgentThresholdAlgorithm(other),
        endpointIndexerAlgorithm(other.endpointIndexerAlgorithm) { }

    virtual ~_selectChargingTaskToAgentThresholdAlgorithm() { }

    virtual bool solve(
            const _token& token,
            const _agent& agent,
            _task& task,
            _stepPath& path) const;
    
    
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm) {
        this->endpointIndexerAlgorithm = endpointIndexerAlgorithm;
    }

private:

    _endpointIndexerAlgorithm& endpointIndexerAlgorithm;
    
};

#endif /* _SELECTCHARGINGTASKTOAGENTTHRESHOLDALGORITHM_H */

