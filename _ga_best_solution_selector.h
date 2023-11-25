/* 
 * File:   _ga_best_solution_selector.h
 * Author: ronaldo
 *
 * Created on 20 de novembro de 2023, 15:23
 */

#ifndef _GA_BEST_SOLUTION_SELECTOR_H
#define _GA_BEST_SOLUTION_SELECTOR_H

#include "_ga_select_solution.h"
#include "_ga_balanced_solution.h"

class _ga_best_solution_selector: public _ga_select_solution{

public:
    
    _ga_best_solution_selector(_ga_solution::EvalType evaltype = _ga_solution::EvalType::balanced);
    
    _ga_best_solution_selector(const _ga_best_solution_selector&);
    
    virtual ~_ga_best_solution_selector();
    
    virtual _ga_solution* select(const _ga_token& , const _ga_objective_function&, const std::vector<_ga_solution*>&) const;
    
private:
    
    _ga_solution* select_makespan(const _ga_token& , const _ga_objective_function&, const std::vector<_ga_solution*>&) const;
    _ga_solution* select_energy(const _ga_token& , const _ga_objective_function&, const std::vector<_ga_solution*>&) const;
    
    _ga_solution::EvalType evaltype;
    _ga_balanced_solution balanced_solution;
        
};

#endif /* _GA_BEST_SOLUTION_SELECTOR_H */

