/* 
 * File:   _ga_estimate_of_path_count.h
 * Author: ronaldo
 *
 * Created on 19 de novembro de 2023, 15:28
 */

#ifndef _GA_ESTIMATE_OF_PATH_COUNT_H
#define _GA_ESTIMATE_OF_PATH_COUNT_H

#include "_ga_objective_function.h"
#include "_task_path.h"

class _ga_estimate_of_path_count : public _ga_objective_function{
public:
    
    
    _ga_estimate_of_path_count(
            bool current, 
            unsigned makespan_penalty, 
            unsigned pickup_energy_penalty,
            unsigned delivery_energy_penalty) :
                current(current), 
                makespan_penalty(makespan_penalty),
                pickup_energy_penalty(pickup_energy_penalty), 
                delivery_energy_penalty(delivery_energy_penalty){ }

    _ga_estimate_of_path_count(const _ga_estimate_of_path_count& other) :
                current(other.current), 
                makespan_penalty(other.makespan_penalty), 
                pickup_energy_penalty(other.pickup_energy_penalty),
                delivery_energy_penalty(other.delivery_energy_penalty){ }

    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;

private:
        
    const bool current;
    
    const unsigned 
                makespan_penalty,
                pickup_energy_penalty,
                delivery_energy_penalty;
    
};

#endif /* _GA_ESTIMATE_OF_PATH_COUNT_H */

