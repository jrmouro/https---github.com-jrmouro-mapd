/* 
 * File:   _selectBackwardTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 12 de setembro de 2023, 10:00
 */

#ifndef _SELECTBACKWARDTASKTOAGENTALGORITHM_H
#define _SELECTBACKWARDTASKTOAGENTALGORITHM_H

#include "_token.h"
#include "_site.h"
#include "_stepPath.h"

class _token;
class _agent;
class _task;
class _stepPath;
class _taskPathToAgentAlgorithm;
class _taskIndexerAlgorithm;

class _selectBackwardTaskToAgentAlgorithm{
    
public:
    
    _selectBackwardTaskToAgentAlgorithm(    
        const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm, 
        _taskIndexerAlgorithm& taskIndexerAlgorithm,
        float delivery_threshold);

    _selectBackwardTaskToAgentAlgorithm(const _selectBackwardTaskToAgentAlgorithm& other);
    
    virtual ~_selectBackwardTaskToAgentAlgorithm(){}
    
    virtual bool solve(
        const _token& token, 
        const _agent& agent, 
        _task& originalTask, 
        _task& selectedTask, 
        _task& pendingTask,
        _stepPath& selectedPath) const;
                
    
    void setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm);
    
    void setDelivery_threshold(float delivery_threshold) {
        this->delivery_threshold = delivery_threshold;
    }
    
protected:
    float   delivery_threshold;
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm;
    _taskIndexerAlgorithm& taskIndexerAlgorithm;
    
};

#endif /* _SELECTBACKWARDTASKTOAGENTALGORITHM_H */

