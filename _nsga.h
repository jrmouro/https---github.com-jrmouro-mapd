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

#include "_agentsTasksAllocator.h"

class _ga_token;
class _ga_population;
class _ga_solution;
class _allocation;

class _nsga : public _agentsTasksAllocator{
    
public:
    
    _nsga(
        const std::function<bool(const _ga_solution&, unsigned)>& stopCondition,
        unsigned population_size_max, 
        unsigned population_size_min, 
        unsigned mutation_children_distribution_size = 3,
        unsigned generic_distribution_size = 100000000,        
        unsigned seed = 1);
            
    _nsga(const _nsga& other);

    virtual _allocation* solve(const _ga_token& token) const;   
    
    void setSeed(unsigned seed) {
        this->seed = seed;
    }

    
protected:
    
    virtual void solve(const _ga_token& token, _ga_solution& solution) const;
    
    virtual void expand_population(const _ga_token& token, std::default_random_engine&, _ga_population& population) const;
    
    virtual _ga_solution* reduce_population(const _ga_token& token, std::default_random_engine& generator, _ga_population& population) const;
    
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
    
    
    unsigned 
        population_size_max, 
        population_size_min,
        mutation_children_distribution_size,
        generic_distribution_size,
        seed;
    

    const std::function<bool(const _ga_solution&, unsigned)> stopCondition;
    
};

#endif /* _NSGA_H */

