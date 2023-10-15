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

class _greedy_agentsTasksAllocator : public _agentsTasksAllocator{
public:
    virtual void solve(const _ga_token& token, const _map&, const _stepMap&, std::unordered_map<int, std::vector<int>>&) const {
        
        std::map<int, unsigned> span_map, cost_task_map;
        unsigned span = 0;
        
        token.listConstAgents([&span_map, &span](const _ga_agent& agent){
            
            unsigned gstep = agent.getGoalSite().GetStep();            
            
            span_map.insert(std::pair<int, unsigned>(agent.id(), gstep));
            
            span = std::max(span, gstep);
            
            return false;
            
        });
        
        
        token.listPendingTasks([](const _task& task){
            
            
            return false;
            
        });
        
        

        
    }
};

#endif /* _GREEDY_AGENTSTASKSALLOCATOR_H */

