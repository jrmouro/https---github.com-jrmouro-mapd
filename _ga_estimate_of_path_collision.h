/* 
 * File:   _ga_estimate_of_path_collision.h
 * Author: ronaldo
 *
 * Created on 19 de novembro de 2023, 15:28
 */

#ifndef _GA_ESTIMATE_OF_PATH_COLLISION_H
#define _GA_ESTIMATE_OF_PATH_COLLISION_H

#include "_ga_objective_function.h"
#include "_task_path.h"

class _ga_estimate_of_path_collision : public _ga_objective_function{
public:
    
    _ga_estimate_of_path_collision(const _task_path& task_path) :
    task_path(task_path) {}
    
    _ga_estimate_of_path_collision(const _ga_estimate_of_path_collision& other) :
    task_path(other.task_path) {}
    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;

private:
        
    const _task_path& task_path;
    
};

#endif /* _GA_ESTIMATE_OF_PATH_COLLISION_H */

