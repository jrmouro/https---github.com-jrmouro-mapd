/* 
 * File:   _ga_objective_function.h
 * Author: ronaldo
 *
 * Created on 19 de outubro de 2023, 14:41
 */

#ifndef _GA_OBJECTIVE_FUNCTION_H
#define _GA_OBJECTIVE_FUNCTION_H

class _ga_solution;

class _ga_objective_function {
public:
    
    virtual float solve(const _ga_solution&, const std::map<_ga_solution::EvalType, unsigned>&) const = 0;

};

#endif /* _GA_OBJECTIVE_FUNCTION_H */

