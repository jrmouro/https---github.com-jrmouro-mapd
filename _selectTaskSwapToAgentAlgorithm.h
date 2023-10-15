/* 
 * File:   _selectTaskSwapToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 25 de setembro de 2023, 14:55
 */

#ifndef _SELECTTASKSWAPTOAGENTALGORITHM_H
#define _SELECTTASKSWAPTOAGENTALGORITHM_H

#include <vector>
#include "_taskToAgentAlgorithm.h"
#include "_chargingTaskToAgentAlgorithm.h"
#include "_selectRestEndpointToAgentAlgorithm.h"
#include "_selectChargingEndpointToAgentAlgorithm.h"

class _token;
class _agent; 
class _task;
class _stepPath;
class _thresholdAlgorithm;
class _pathToAgentAlgorithm;
class _taskPathToAgentAlgorithm;
class _taskIndexerAlgorithm;
class _endpointIndexerAlgorithm;
class _taskSwapInfo;
class _swapInfo;

class _selectTaskSwapToAgentAlgorithm {
    
public:
    
    _selectTaskSwapToAgentAlgorithm(
            const _pathToAgentAlgorithm&, 
            const _taskPathToAgentAlgorithm&, 
            _endpointIndexerAlgorithm&,
            _taskIndexerAlgorithm&);
        
    _selectTaskSwapToAgentAlgorithm(const _selectTaskSwapToAgentAlgorithm& other);
    
    virtual ~_selectTaskSwapToAgentAlgorithm();
    
    virtual bool solve(
        const _token&, 
        const _agent&, 
        _task&, _stepPath&, 
        std::vector<_swapInfo>&) const;
    
    void setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm);    
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm);
    
protected:
    
    _taskToAgentAlgorithm taskToAgentAlgorithm;
    _chargingTaskToAgentAlgorithm chargingTaskToAgentAlgorithm;
    _selectChargingEndpointToAgentAlgorithm selectChargingEndpointToAgentAlgorithm;
    _selectRestEndpointToAgentAlgorithm selectRestEndpointToAgentAlgorithm;
    _taskIndexerAlgorithm& taskIndexerAlgorithm;

};

#endif /* _SELECTTASKSWAPTOAGENTALGORITHM_H */

