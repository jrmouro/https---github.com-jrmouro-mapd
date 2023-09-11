/* 
 * File:   _taskIndexer.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 15:09
 */

#ifndef _TASKINDEXER_H
#define _TASKINDEXER_H

#include <vector>
#include "_task.h"
#include "_agent.h"

class _token;
class _taskIndexerAlgorithm {
public:
    
    virtual _taskIndexerAlgorithm* getInstance() const = 0;
    
    virtual void solve(
        const _token& token,
        const _task& task, 
        const _site& endPointReference, 
        std::vector<_task>& taskIndex) const = 0;
    
    void solve(
        const _token& token,
        const _task& task, 
        const _agent& agent, 
        std::vector<_task>& taskIndex) const;
    
};

#endif /* _TASKINDEXER_H */

