/* 
 * File:   _ga_solution_comparison.h
 * Author: ronaldo
 *
 * Created on 19 de outubro de 2023, 15:25
 */

#ifndef _GA_SOLUTION_COMPARISON_H
#define _GA_SOLUTION_COMPARISON_H

#include "_ga_solution.h"


class _ga_solution;

class _ga_solution_comparison {
public:
    
    _ga_solution_comparison(_ga_solution::EvalType evalType);
    
    _ga_solution_comparison(const _ga_solution_comparison& other);
    
    virtual ~_ga_solution_comparison();
    
    bool operator() (const _ga_solution& lhs, const _ga_solution&rhs) const;
    
private:
    
    const _ga_solution::EvalType evalType;

};

#endif /* _GA_SOLUTION_COMPARISON_H */

