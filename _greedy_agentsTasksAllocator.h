/* 
 * File:   _greedy_agentsTasksAllocator.h
 * Author: ronaldo
 *
 * Created on 14 de outubro de 2023, 16:22
 */

#ifndef _GREEDY_AGENTSTASKSALLOCATOR_H
#define _GREEDY_AGENTSTASKSALLOCATOR_H

#include "_agentsTasksAllocator.h"
#include "_ga_token.h"
#include "_endpointsDistanceAlgorithm.h"
#include <vector>
#include <unordered_map>

class _ga_token;
class _ga_agent;
class _task;

class _greedy_agentsTasksAllocator : public _agentsTasksAllocator{
    
public:
    
    virtual void solve(
        const _ga_token& token, 
        const _endpointsDistanceAlgorithm&, 
        std::unordered_map<int, std::vector<int>>&) const;
    
    virtual void solve(
        const std::vector<const _ga_agent*>&, 
        const std::vector<const _task*>&, 
        const _endpointsDistanceAlgorithm&, 
        std::unordered_map<const _ga_agent*, std::vector<const _task*>>&) const;
};

#endif /* _GREEDY_AGENTSTASKSALLOCATOR_H */

