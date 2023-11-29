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
    
    _ga_estimate_of_path_collision(const _task_path& task_path, unsigned makespan_penalty, unsigned pickup_energy_penalty, unsigned delivery_energy_penalty) :
            task_path(task_path), 
            makespan_penalty(makespan_penalty), 
            pickup_energy_penalty(pickup_energy_penalty), 
            delivery_energy_penalty(delivery_energy_penalty) { }
    
    _ga_estimate_of_path_collision(const _ga_estimate_of_path_collision& other) :
            task_path(other.task_path), 
            makespan_penalty(other.makespan_penalty), 
            pickup_energy_penalty(other.pickup_energy_penalty), 
            delivery_energy_penalty(other.delivery_energy_penalty) { }
    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;

private:
        
    const _task_path& task_path;
    
    unsigned 
                makespan_penalty,
                pickup_energy_penalty,
                delivery_energy_penalty;
    
};

#endif /* _GA_ESTIMATE_OF_PATH_COLLISION_H */

