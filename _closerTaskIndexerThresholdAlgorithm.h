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
    
    _closerTaskIndexerThresholdAlgorithm(unsigned num_task_threshold) :
    num_task_threshold(num_task_threshold) { }

    _closerTaskIndexerThresholdAlgorithm(const _closerTaskIndexerThresholdAlgorithm& other) :
    num_task_threshold(other.num_task_threshold) { }
    
    virtual _taskIndexerAlgorithm* getInstance() const;
    
    virtual void solve(
            const _token& token,
            const _task& task, 
            const _site& endPointReference, 
            std::vector<_task>& taskIndex) const;
    
    unsigned getNum_task_threshold() const {
        return num_task_threshold;
    }

    void setNum_task_threshold(unsigned num_task_threshold) {
        this->num_task_threshold = num_task_threshold;
    }
    
private:
    
    unsigned num_task_threshold;

};

#endif /* _CLOSERTASKINDEXERTHRESHOLDALGORITHM_H */

