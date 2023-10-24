/* 
 * File:   _ga_pseudo_solution.h
 * Author: ronaldo
 *
 * Created on 16 de outubro de 2023, 09:24
 */

#ifndef _GA_PSEUDO_SOLUTION_H
#define _GA_PSEUDO_SOLUTION_H

#include "_ga_token.h"


class _ga_pseudo_solution {
public:
    
    _ga_pseudo_solution();
    
    _ga_pseudo_solution(const _ga_token& token);
    
    _ga_pseudo_solution(const _ga_pseudo_solution& other);
    
    virtual ~_ga_pseudo_solution();
            
    void listConstAgents(const std::function<bool(const _ga_agent*)>&) const;
    void listConstTasks(const std::function<bool(const _task*)>&) const;
    
protected:
    
    virtual void disturb(unsigned, unsigned, unsigned);  
    virtual void disturb(unsigned); 
    
    void swapAgentsByIndexes(unsigned indexAgentA, unsigned indexAgentB);    
    void swapTasksByIndexes(unsigned indexTaskA, unsigned indexTaskB);
    
    void swapAgents(std::set<unsigned> swapSet);      
    void swapTasks(std::set<unsigned> swapSet); 
    
    void swap(
        std::set<unsigned> swapAgentsSet, 
        std::set<unsigned> swapTasksSet);       
    
    friend void crossover(
        const _ga_pseudo_solution& parentA,
        const _ga_pseudo_solution& parentB, 
        _ga_pseudo_solution& childA, 
        _ga_pseudo_solution& childB);
    
    void reset(const _ga_token& token);    
    bool empty() const;    
    void clear(unsigned agentsSize = 0, unsigned tasksSize = 0);
    
    bool isConsistentWith(const _ga_pseudo_solution& other) const;
        
    friend void agentsCrossover(
        const _ga_pseudo_solution& parentA,
        const _ga_pseudo_solution& parentB, 
        _ga_pseudo_solution& childA, 
        _ga_pseudo_solution& childB);
    
    friend void tasksCrossover(
        const _ga_pseudo_solution& parentA,
        const _ga_pseudo_solution& parentB, 
        _ga_pseudo_solution& childA, 
        _ga_pseudo_solution& childB);
            
    std::vector<const _ga_agent*> agents;
    std::vector<const _task*> tasks;
    
};

#endif /* _GA_PSEUDO_SOLUTION_H */

