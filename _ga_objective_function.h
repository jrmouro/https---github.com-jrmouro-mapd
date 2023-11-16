/* 
 * File:   _ga_objective_function.h
 * Author: ronaldo
 *
 * Created on 14 de novembro de 2023, 15:58
 */

#ifndef _GA_OBJECTIVE_FUNCTION_H
#define _GA_OBJECTIVE_FUNCTION_H

#include "_ga_token.h"
#include "_ga_solution.h"

class _ga_objective_function {
public:
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const = 0;
    unsigned eval(const _ga_token& token, _ga_solution&, const _ga_solution::EvalType&)const;
};

#endif /* _GA_OBJECTIVE_FUNCTION_H */

