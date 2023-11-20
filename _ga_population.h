/* 
 * File:   _ga_population.h
 * Author: ronaldo
 *
 * Created on 19 de outubro de 2023, 14:49
 */

#ifndef _GA_POPULATION_H
#define _GA_POPULATION_H

#include <set>
#include <functional>
#include <random>

#include "_ga_solution.h"


class _ga_population {
public:
    
    _ga_population(const _ga_solution&, std::default_random_engine& generator, unsigned, unsigned);
    
    _ga_population(unsigned, unsigned);
    
    _ga_population(const _ga_population& other);    
    
    virtual ~_ga_population();
    
    bool add(_ga_solution*);
    bool remove(_ga_solution*);
        
    void listConstSolutions(const std::function<bool(unsigned, const _ga_solution&)>) const; 
    void listSolutions(const std::function<bool(unsigned, _ga_solution*)>) const; 
    
    virtual void populate_random(const _ga_solution&, std::default_random_engine& generator);
        
    virtual _ga_solution* get_random(std::default_random_engine& generator) const;
    
    bool isExpandable()const;    
    bool isReducible()const;
    
    const unsigned getSize_max() const;
    const unsigned getSize_min() const;
    const unsigned getSize_cur() const;
    
    friend std::ostream& operator<<(std::ostream& os, const _ga_population& obj);
    
private:
    
    const unsigned size_max, size_min;
    std::set<_ga_solution*> solutions;
    
};

#endif /* _GA_POPULATION_H */

