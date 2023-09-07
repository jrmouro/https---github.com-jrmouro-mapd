/* 
 * File:   _selectTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 05:07
 */

#ifndef _SELECTTASKTOAGENTALGORITHM_H
#define _SELECTTASKTOAGENTALGORITHM_H

#include "_taskPathToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"

class _selectTaskToAgentAlgorithm {
    
public:
    
    _selectTaskToAgentAlgorithm(
            const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
            const _taskIndexerAlgorithm& taskIndexerAlgorithm) :
        taskPathToAgentAlgorithm(taskPathToAgentAlgorithm),
        taskIndexerAlgorithm(taskIndexerAlgorithm){ }
        
    _selectTaskToAgentAlgorithm(const _selectTaskToAgentAlgorithm& other) : 
        taskPathToAgentAlgorithm(other.taskPathToAgentAlgorithm),
        taskIndexerAlgorithm(other.taskIndexerAlgorithm){ }
    
    virtual ~_selectTaskToAgentAlgorithm(){}
    
    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _task& selectedTask, 
            _stepPath& selectedPath) const;
    
protected:
    
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm;
    const _taskIndexerAlgorithm& taskIndexerAlgorithm;

};

#endif /* _SELECTTASKTOAGENTALGORITHM_H */

