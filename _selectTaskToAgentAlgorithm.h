/* 
 * File:   _selectTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 05:07
 */

#ifndef _SELECTTASKTOAGENTALGORITHM_H
#define _SELECTTASKTOAGENTALGORITHM_H

class _token;
class _agent; 
class _task;
class _stepPath;
class _thresholdAlgorithm;
class _taskPathToAgentAlgorithm;
class _taskIndexerAlgorithm;

class _selectTaskToAgentAlgorithm {
    
public:
    
    _selectTaskToAgentAlgorithm(const _taskPathToAgentAlgorithm&, _taskIndexerAlgorithm&);
        
    _selectTaskToAgentAlgorithm(const _selectTaskToAgentAlgorithm&);
    
    virtual ~_selectTaskToAgentAlgorithm(){}
    
    virtual bool solve(const _token&, const _agent&, _task&, _stepPath&) const;
    
    void setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm);
    
protected:
    
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm;
    _taskIndexerAlgorithm& taskIndexerAlgorithm;

};

#endif /* _SELECTTASKTOAGENTALGORITHM_H */

