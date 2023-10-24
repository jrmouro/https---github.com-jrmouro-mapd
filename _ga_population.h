/* 
 * File:   _ga_population.h
 * Author: ronaldo
 *
 * Created on 19 de outubro de 2023, 14:49
 */

#ifndef _GA_POPULATION_H
#define _GA_POPULATION_H

#include <vector>
#include <functional>

#include "_ga_solution.h"


class _ga_population {
public:
    
    _ga_population(const _ga_solution&, unsigned, unsigned, unsigned);
    
    _ga_population(unsigned, unsigned);
    
    _ga_population(const _ga_population& other);    
    
    virtual ~_ga_population();
    
    void add(_ga_solution*);
        
    void listConstSolutions(const std::function<bool(unsigned, const _ga_solution&)>) const; 
    
    virtual void populate_random(const _ga_solution&, unsigned);
    virtual void populate_random(_ga_solution*, unsigned);
        
    virtual _ga_solution* get_random(unsigned) const;
    
private:
    
    const unsigned size_max, size_min;
    std::vector<_ga_solution*> solutions;
    
};

#endif /* _GA_POPULATION_H */

