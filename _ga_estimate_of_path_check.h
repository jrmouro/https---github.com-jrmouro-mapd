/* 
 * File:   _ga_estimate_of_path_check.h
 * Author: ronaldo
 *
 * Created on 19 de novembro de 2023, 15:28
 */

#ifndef _GA_ESTIMATE_OF_PATH_CHECK_H
#define _GA_ESTIMATE_OF_PATH_CHECK_H

#include "_ga_estimative_of_path.h"
#include "_task_path.h"

class _ga_estimate_of_path_check : public _ga_estimative_of_path{
public:
    
    _ga_estimate_of_path_check(
            const bool current,
            const unsigned makespan_penalty, 
            const unsigned pickup_energy_penalty, 
            const unsigned delivery_energy_penalty ) :
        _ga_estimative_of_path(
                makespan_penalty, 
                pickup_energy_penalty, 
                delivery_energy_penalty), current(current) { }

    
    _ga_estimate_of_path_check(const _ga_estimate_of_path_check& other) :
    _ga_estimative_of_path(other), current(other.current) { }

    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;

private:
        
    const bool current;
    
};

#endif /* _GA_ESTIMATE_OF_PATH_CHECK_H */

