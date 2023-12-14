/* 
 * File:   _ga_estimative_of_path.h
 * Author: ronaldo
 *
 * Created on 9 de dezembro de 2023, 21:57
 */

#ifndef _GA_ESTIMATIVE_OF_PATH_H
#define _GA_ESTIMATIVE_OF_PATH_H
#include "_ga_objective_function.h"
#include <vector>
#include "_task_path.h"

class _ga_estimative_of_path  : public _ga_objective_function{
    
public:
    
    _ga_estimative_of_path(const unsigned makespan_penalty, const unsigned pickup_energy_penalty, const unsigned delivery_energy_penalty) :
    makespan_penalty(makespan_penalty), pickup_energy_penalty(pickup_energy_penalty), delivery_energy_penalty(delivery_energy_penalty) {
    }
    
    _ga_estimative_of_path(const _ga_estimative_of_path& other) :
    makespan_penalty(other.makespan_penalty), pickup_energy_penalty(other.pickup_energy_penalty), delivery_energy_penalty(other.delivery_energy_penalty) {
    }
    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const = 0;
    
    protected:
        
        void pathSwap(
            const _ga_token&, 
            const _ga_solution&, 
            unsigned&,
            unsigned&,
            std::map<const _ga_agent*, _path>&,
            std::vector<std::vector<const _site*>>&,
            std::vector<std::vector<_task_path::PathType>>&) const;
        
        void path(
            const _ga_token&, 
            const _ga_solution&,
            unsigned&,
            unsigned&,
            std::map<const _ga_agent*, _path>&,
            std::vector<std::vector<const _site*>>&,
            std::vector<std::vector<_task_path::PathType>>&)const;
        
        void pathSwapTaskEndpoint(
            const _ga_token&, 
            const _ga_solution&, 
            unsigned&,
            unsigned&,
            std::map<const _ga_agent*, _path>&,
            std::vector<std::vector<const _site*>>&,
            std::vector<std::vector<_task_path::PathType>>&,
            std::vector<std::vector<unsigned>>&,
            std::vector<std::vector<unsigned>>&) const;
        
        void pathTaskEndpoint(
            const _ga_token&, 
            const _ga_solution&,
            unsigned&,
            unsigned&,
            std::map<const _ga_agent*, _path>&,
            std::vector<std::vector<const _site*>>&,
            std::vector<std::vector<_task_path::PathType>>&,
            std::vector<std::vector<unsigned>>&,
            std::vector<std::vector<unsigned>>&)const;
        
        const unsigned 
                makespan_penalty,
                pickup_energy_penalty,
                delivery_energy_penalty;
        
};

#endif /* _GA_ESTIMATIVE_OF_PATH_H */

