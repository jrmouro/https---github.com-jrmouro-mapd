/* 
 * File:   _ga_solutionAllocator.h
 * Author: ronaldo
 *
 * Created on 26 de outubro de 2023, 10:07
 */

#ifndef _GA_SOLUTIONALLOCATOR_H
#define _GA_SOLUTIONALLOCATOR_H

#include "_agentsTasksAllocator.h"

#include <set>


class _ga_solution;
class _ga_objective_function;

class _ga_solutionAllocator : public _agentsTasksAllocator{

public:
    
    _ga_solutionAllocator(
            const std::string id, 
            unsigned solution_validity);
    
    _ga_solutionAllocator(const _ga_solutionAllocator& other);
    
    virtual ~_ga_solutionAllocator();

    virtual _allocation* borrow(const _ga_token&) ;
    virtual _allocation* restore(const _ga_token&, _allocation*);
    virtual void giveBack(_allocation*) ;
    virtual _allocation* borrowClone(_allocation*) ;
    virtual _agentsTasksAllocator* emptyClone() const;
           
    
protected:
    
    std::set<_ga_solution*> borrowed;
    unsigned solution_validity;
        
};

#endif /* _GA_SOLUTIONALLOCATOR_H */

