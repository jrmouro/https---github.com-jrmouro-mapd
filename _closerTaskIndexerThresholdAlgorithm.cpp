/* 
 * File:   _closerTaskIndexerThresholdAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 11 de setembro de 2023, 07:35
 */

#include "_closerTaskIndexerThresholdAlgorithm.h"

_taskIndexerAlgorithm* _closerTaskIndexerThresholdAlgorithm::getInstance() const{
    return new _closerTaskIndexerThresholdAlgorithm(*this);
}

void _closerTaskIndexerThresholdAlgorithm::solve(
        const _token& token,
        const _task& task,
        const _site& endPointReference,
        std::vector<_task>& taskIndex) const {

    _closerTaskIndexerAlgorithm::solve(token, task, endPointReference, taskIndex);
    
    if(taskIndex.size() > task_threshold)
        taskIndex.pop_back();

}

