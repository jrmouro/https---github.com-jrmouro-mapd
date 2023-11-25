/* 
 * File:   _ga_estimate_of_path_check.h
 * Author: ronaldo
 *
 * Created on 19 de novembro de 2023, 15:28
 */

#ifndef _GA_ESTIMATE_OF_PATH_CHECK_H
#define _GA_ESTIMATE_OF_PATH_CHECK_H

#include "_ga_objective_function.h"
#include "_task_path.h"

class _ga_estimate_of_path_check : public _ga_objective_function{
public:
    
    _ga_estimate_of_path_check(const _task_path& task_path) :
    task_path(task_path) {}
    
    _ga_estimate_of_path_check(const _ga_estimate_of_path_check& other) :
    task_path(other.task_path) { }

    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;

private:
        
    const _task_path& task_path;
    
};

#endif /* _GA_ESTIMATE_OF_PATH_CHECK_H */

