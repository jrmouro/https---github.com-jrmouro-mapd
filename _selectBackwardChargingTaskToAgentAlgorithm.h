/* 
 * File:   _selectBackwardChargingTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 12 de setembro de 2023, 15:32
 */

#ifndef _SELECTBACKWARDCHARGINGTASKTOAGENTALGORITHM_H
#define _SELECTBACKWARDCHARGINGTASKTOAGENTALGORITHM_H

#include "_selectBackwardTaskToAgentAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"

//class _selectBackwardTaskToAgentAlgorithm;
//class _endpointIndexerAlgorithm;
class _selectBackwardChargingTaskToAgentAlgorithm : public _selectBackwardTaskToAgentAlgorithm{
public:
    
    _selectBackwardChargingTaskToAgentAlgorithm(
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm, 
            _taskIndexerAlgorithm& taskIndexerAlgorithm,
            _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
            float pickup_threshold,
            float delivery_threshold);
    
    _selectBackwardChargingTaskToAgentAlgorithm(
    const _selectBackwardChargingTaskToAgentAlgorithm& other);

    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _task& originalTask, 
            _task& selectedTask,            
            _task& pendingTask,
            _stepPath& selectedPath) const;
    
    virtual ~_selectBackwardChargingTaskToAgentAlgorithm(){}
    
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm) {
        this->endpointIndexerAlgorithm = endpointIndexerAlgorithm;
    }
    
private:

    _endpointIndexerAlgorithm& endpointIndexerAlgorithm;
    
};

#endif /* _SELECTBACKWARDCHARGINGTASKTOAGENTALGORITHM_H */

