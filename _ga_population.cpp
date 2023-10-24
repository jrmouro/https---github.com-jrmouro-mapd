/* 
 * File:   _ga_population.cpp
 * Author: ronaldo
 * 
 * Created on 19 de outubro de 2023, 14:49
 */

#include "_ga_population.h"

_ga_solution* _ga_population::get_random(unsigned seed) const{
    
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<unsigned> distribution(0, solutions.size() - 1);
    
    return solutions.at(distribution(generator));
    
}

void _ga_population::populate_random(const _ga_solution& solution, unsigned seed){
    
    if(size_min < solutions.size()){
        
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<unsigned> distribution(1,1000000);
        
        solutions.push_back(new _ga_solution(solution));        
    
        while(size_min < solutions.size()){

            solutions.push_back(solution.randon(distribution(generator)));

        }
        
    }
    
}

void _ga_population::populate_random(_ga_solution* solution, unsigned seed){
    
    if(size_min < solutions.size()){
        
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<unsigned> distribution(1,1000000);
        
        solutions.push_back(solution);        
    
        while(size_min < solutions.size()){

            solutions.push_back(solution.randon(distribution(generator)));

        }
        
    }
    
}

_ga_population::_ga_population(
        const _ga_solution& solution, 
        unsigned seed, 
        unsigned size_max,
        unsigned size_min): size_min(size_min), size_max(size_max){
    
    populate_random(solution, seed);        
    
}

_ga_population::_ga_population(
        unsigned size_max,
        unsigned size_min): size_min(size_min), size_max(size_max) {}

_ga_population::_ga_population(const _ga_population& other) :
        size_max(other.size_max),
        size_min(other.size_min){

    for (auto solution : other.solutions) {
        
        solutions.push_back(new _ga_solution(*solution));

    }

}

_ga_population::~_ga_population(){

    for (auto solution : solutions) {

        delete solution;
        
    }

}

void _ga_population::add(_ga_solution* solution){
    
    if(size_max < solutions.size())    
        solutions.push_back(solution);
    
}

void _ga_population::listConstSolutions(const std::function<bool(unsigned, const _ga_solution&)> function) const{
    
    unsigned index = 0;
    for (auto solution : solutions) {

        if(function(index++, *solution))break;
        
    }

}