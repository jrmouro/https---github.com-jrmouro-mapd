/* 
 * File:   _closerTaskIndexerThresholdAlgorithm.h
 * Author: ronaldo
 *
 * Created on 11 de setembro de 2023, 07:35
 */

#ifndef _CLOSERTASKINDEXERTHRESHOLDALGORITHM_H
#define _CLOSERTASKINDEXERTHRESHOLDALGORITHM_H

#include "_closerTaskIndexerAlgorithm.h"

class _closerTaskIndexerThresholdAlgorithm : public _closerTaskIndexerAlgorithm{
public:
    
    _closerTaskIndexerThresholdAlgorithm(unsigned task_threshold) :
    task_threshold(task_threshold) { }

    _closerTaskIndexerThresholdAlgorithm(const _closerTaskIndexerThresholdAlgorithm& other) :
    task_threshold(other.task_threshold) { }
    
    virtual _taskIndexerAlgorithm* getInstance() const;
    
    virtual void solve(
            const _token& token,
            const _task& task, 
            const _site& endPointReference, 
            std::vector<_task>& taskIndex) const;
    
    unsigned getTaskThreshold() const {
        return task_threshold;
    }

    void setTaskThreshold(unsigned task_threshold) {
        this->task_threshold = task_threshold;
    }
    
private:
    
    unsigned task_threshold;

};

#endif /* _CLOSERTASKINDEXERTHRESHOLDALGORITHM_H */

