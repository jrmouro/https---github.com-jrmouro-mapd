/* 
 * File:   _taskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 26 de setembro de 2023, 14:13
 */

#ifndef _TASKTOAGENTALGORITHM_H
#define _TASKTOAGENTALGORITHM_H

class _token;
class _agent; 
class _task;
class _stepPath;
class _stepSite;
class _thresholdAlgorithm;
class _taskPathToAgentAlgorithm;

class _taskToAgentAlgorithm {
    
public:
    
    _taskToAgentAlgorithm(const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm);
    _taskToAgentAlgorithm(const _taskToAgentAlgorithm& other);
    virtual ~_taskToAgentAlgorithm();
    
    virtual bool solve(
        const _token&, 
        const _agent&, 
        const _task&, 
        _stepPath&, 
        _stepSite&,
        _stepSite&) const;
    
private:
    
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm;
    
};

#endif /* _TASKTOAGENTALGORITHM_H */

