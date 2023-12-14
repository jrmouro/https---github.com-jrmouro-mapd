/* 
 * File:   _ga_estimate_of_path_task_endpoint.h
 * Author: ronaldo
 *
 * Created on 1 de dezembro de 2023, 06:57
 */

#ifndef _GA_ESTIMATE_OF_PATH_TASK_ENDPOINT_H
#define _GA_ESTIMATE_OF_PATH_TASK_ENDPOINT_H

#include "_ga_estimative_of_path.h"
#include "_task_path.h"

class _ga_estimate_of_path_task_endpoint : public _ga_estimative_of_path {
public:
    
    _ga_estimate_of_path_task_endpoint(
            const bool current,
            const unsigned makespan_penalty, 
            const unsigned pickup_energy_penalty, 
            const unsigned delivery_energy_penalty ) :
        _ga_estimative_of_path(
                makespan_penalty, 
                pickup_energy_penalty, 
                delivery_energy_penalty), current(current) { }
    
    _ga_estimate_of_path_task_endpoint(const _ga_estimate_of_path_task_endpoint& other) :
    _ga_estimative_of_path(other), current(other.current) { }

    
    virtual ~_ga_estimate_of_path_task_endpoint(){}
    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;
        
private:
    
    const bool current;
};

#endif /* _GA_ESTIMATE_OF_PATH_TASK_ENDPOINT_H */

