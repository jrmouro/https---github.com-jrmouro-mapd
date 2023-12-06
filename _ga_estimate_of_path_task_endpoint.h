/* 
 * File:   _ga_estimate_of_path_task_endpoint.h
 * Author: ronaldo
 *
 * Created on 1 de dezembro de 2023, 06:57
 */

#ifndef _GA_ESTIMATE_OF_PATH_TASK_ENDPOINT_H
#define _GA_ESTIMATE_OF_PATH_TASK_ENDPOINT_H

#include "_ga_objective_function.h"
#include "_task_path.h"

class _ga_estimate_of_path_task_endpoint : public _ga_objective_function {
public:
    
    _ga_estimate_of_path_task_endpoint(const bool current, const unsigned makespan_penalty, const unsigned pickup_energy_penalty, const unsigned delivery_energy_penalty) :
            current(current), 
            makespan_penalty(makespan_penalty), 
            pickup_energy_penalty(pickup_energy_penalty), 
            delivery_energy_penalty(delivery_energy_penalty) { }
    
    _ga_estimate_of_path_task_endpoint(const _ga_estimate_of_path_task_endpoint& other) :
            current(other.current), 
            makespan_penalty(other.makespan_penalty), 
            pickup_energy_penalty(other.pickup_energy_penalty), 
            delivery_energy_penalty(other.delivery_energy_penalty) {}
    
    virtual ~_ga_estimate_of_path_task_endpoint(){}
    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;
        
private:
    
    const bool current;
    
    const unsigned 
                makespan_penalty,
                pickup_energy_penalty,
                delivery_energy_penalty;
};

#endif /* _GA_ESTIMATE_OF_PATH_TASK_ENDPOINT_H */

