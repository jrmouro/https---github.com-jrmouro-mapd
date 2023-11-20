/* 
 * File:   _nsga.h
 * Author: ronaldo
 *
 * Created on 22 de outubro de 2023, 08:31
 */

#ifndef _NSGA_H
#define _NSGA_H


#include <map>
#include <vector>
#include <cmath>
#include <climits>
#include <functional>
#include <random>
#include <ratio>
#include <chrono>

#include "_ga_solutionAllocator.h"
#include "_ga_select_solution.h"

class _ga_token;
class _ga_population;
class _ga_solution;
class _allocation;

class _nsga : public _ga_solutionAllocator{
    
public:
    
    _nsga(
        const std::string& id,
        const _ga_select_solution& select_solution,
        const _ga_objective_function& ga_objective_function, 
        const std::function<bool(const _ga_solution&, unsigned, const std::chrono::duration<double>&)>& stopCondition,
        unsigned population_size_max, 
        unsigned population_size_min, 
        unsigned solution_validity = UINT_MAX,
        float population_mutation_rate = .4f,
        float agents_crossover_point_rate = .5f,
        float tasks_crossover_point_rate = .5f,
        float agents_mutation_rate = .2f,
        float tasks_mutation_rate = .2f,      
        unsigned seed = 1);
            
    _nsga(const _nsga& other);
    
    virtual ~_nsga();

    virtual _allocation* borrow(const _ga_token& token) ; 
    virtual _allocation* restore(const _ga_token&, _allocation*);
    virtual _agentsTasksAllocator* emptyClone() const;
    
    void setSeed(unsigned seed) {
        this->seed = seed;
    }

    
protected:
    
    virtual void solve(const _ga_token& token, _ga_solution& solution) const;
    
    virtual void expand_population(
        const _ga_token& token,
        const _ga_solution&,
        std::default_random_engine&, 
        _ga_population& population) const;
    
    virtual void reduce_population(
        const _ga_token& token, 
        std::default_random_engine& generator, 
        _ga_population& population,
        std::vector<_ga_solution*>& border_zero) const;
    
    virtual std::pair<_ga_solution*, _ga_solution*> crossover_select_parents(
        const _ga_token& token, 
        const _ga_population& population,
        std::default_random_engine& generator,
        const std::map<_ga_solution*, unsigned>& solution_border_map) const;
        
    virtual void tournament_selection(
        const _ga_token& token, 
        const _ga_population& population, 
        const std::map<_ga_solution*, unsigned>& solution_border_map,
        std::default_random_engine& generator,
        _ga_solution** solution) const;
    
    virtual _ga_solution* crowding_distance(
            const _ga_token& token,
            const std::vector<_ga_solution*>& border, 
            std::map<_ga_solution*, float>& solution_distance_map) const;
    
    virtual void nds_population(
            const _ga_token& token, 
            const _ga_population& population, 
            std::vector<std::vector<_ga_solution*>>& borders,
            std::map<_ga_solution*, unsigned>& solution_border_map) const ;
    
    
    float   population_mutation_rate,
            agents_crossover_point_rate,
            tasks_crossover_point_rate,
            agents_mutation_rate,
            tasks_mutation_rate;
    
    unsigned 
        population_size_max, 
        population_size_min,
        generic_distribution_size = 100000000,
        seed;
    
    
    const _ga_objective_function& objective_function;  
    const _ga_select_solution& select_solution;

    const std::function<bool(const _ga_solution&, unsigned, const std::chrono::duration<double>&)> stopCondition;
    
};

#endif /* _NSGA_H */

