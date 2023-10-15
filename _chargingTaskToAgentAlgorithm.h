/* 
 * File:   _chargingTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 1 de outubro de 2023, 09:04
 */

#ifndef _CHARGINGTASKTOAGENTALGORITHM_H
#define _CHARGINGTASKTOAGENTALGORITHM_H

class _token;
class _agent; 
class _task;
class _stepPath;
class _stepSite;
class _thresholdAlgorithm;
class _taskPathToAgentAlgorithm;
class _endpointIndexerAlgorithm;

class _chargingTaskToAgentAlgorithm {
    
public:
    
    _chargingTaskToAgentAlgorithm(
        const _taskPathToAgentAlgorithm&, 
        _endpointIndexerAlgorithm&);
    
    
    _chargingTaskToAgentAlgorithm(const _chargingTaskToAgentAlgorithm&);
    
    virtual ~_chargingTaskToAgentAlgorithm();
    
    virtual bool solve(
            const _token&,
            const _agent&,
            const _task&,
            _stepPath&,
            _stepSite&,
            _stepSite&) const;
    
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm);
    
private:
    
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm;
    _endpointIndexerAlgorithm& endpointIndexerAlgorithm;

};

#endif /* _CHARGINGTASKTOAGENTALGORITHM_H */

