/* 
 * File:   _closerTaskIndexerAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 15:19
 */

#ifndef _CLOSERTASKINDEXERALGORITHM_H
#define _CLOSERTASKINDEXERALGORITHM_H

#include "_taskIndexerAlgorithm.h"
#include "_endPointsDistanceAlgorithm.h"

class _closerTaskIndexerAlgorithm : public _taskIndexerAlgorithm{
    
public:
    
    _closerTaskIndexerAlgorithm(){ }
            
    _closerTaskIndexerAlgorithm(const _closerTaskIndexerAlgorithm& other) { }
    
    virtual _taskIndexerAlgorithm* getInstance() const;
    
    virtual void solve(
            const _token& token,
            const _task& task, 
            const _site& endPointReference, 
            std::vector<_task>& taskIndex) const;
    
    
};

#endif /* _CLOSERTASKINDEXERALGORITHM_H */

