/* 
 * File:   _ga_estimate_of_path.h
 * Author: ronaldo
 *
 * Created on 19 de novembro de 2023, 15:28
 */

#ifndef _GA_ESTIMATE_OF_PATH_H
#define _GA_ESTIMATE_OF_PATH_H

#include "_ga_objective_function.h"

class _ga_estimate_of_path : public _ga_objective_function{
public:
    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;

};

#endif /* _GA_ESTIMATE_OF_PATH_H */

