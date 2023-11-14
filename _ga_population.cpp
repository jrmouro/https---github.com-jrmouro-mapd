/* 
 * File:   _ga_population.cpp
 * Author: ronaldo
 * 
 * Created on 19 de outubro de 2023, 14:49
 */

#include "_ga_population.h"
#include <random>

_ga_solution* _ga_population::get_random(std::default_random_engine& generator) const{
    
    std::uniform_int_distribution<unsigned> distribution(0, solutions.size() - 1);
    
    unsigned index = distribution(generator);
    
    for (auto solution : solutions) {
        
        if(index-- == 0){
            
            return solution;
            
        }

    }
    
    return nullptr;
    
}

void _ga_population::populate_random(const _ga_solution& solution, std::default_random_engine& generator){
    
    if(size_min > solutions.size()){
        
        std::uniform_int_distribution<unsigned> distribution(1,1000000);
        
        solutions.insert(new _ga_solution(solution));        
    
        while(size_min > solutions.size()){

            solutions.insert(solution.randon(generator));

        }
        
    }
    
}

_ga_population::_ga_population(
        const _ga_solution& solution, 
        std::default_random_engine& generator,
        unsigned size_max,
        unsigned size_min): size_min(size_min), size_max(size_max){
    
    populate_random(solution, generator);        
    
}

_ga_population::_ga_population(
        unsigned size_max,
        unsigned size_min): size_min(size_min), size_max(size_max) {}

_ga_population::_ga_population(const _ga_population& other) :
        size_max(other.size_max),
        size_min(other.size_min){

    for (auto solution : other.solutions) {
        
        solutions.insert(new _ga_solution(*solution));

    }

}

_ga_population::~_ga_population(){

    for (auto solution : solutions) {

        delete solution;
        
    }

}

bool _ga_population::add(_ga_solution* solution){
    
    if(size_max > solutions.size()){    
        auto insit = solutions.insert(solution);
        return insit.second;
    }
    
    return false;
    
}

bool _ga_population::remove(_ga_solution* solution){
    
    if(solutions.size() > size_min){ 
    
        auto it = solutions.find(solution);

        if(it != solutions.end()){

            delete *it;

            solutions.erase(it);

            return true;

        }
    
    }
    
    return false;
    
}

const unsigned _ga_population::getSize_max() const {
    return size_max;
}

const unsigned _ga_population::getSize_min() const {
    return size_min;
}

bool _ga_population::isExpandable()const{
    return solutions.size() < size_max;
}

bool _ga_population::isReducible()const{
    return size_min < solutions.size();
}

void _ga_population::listConstSolutions(const std::function<bool(unsigned, const _ga_solution&)> function) const{
    
    unsigned index = 0;
    for (auto solution : solutions) {

        if(function(index++, *solution))break;
        
    }

}

void _ga_population::listSolutions(const std::function<bool(unsigned, _ga_solution*)> function) const{
    
    unsigned index = 0;
    for (auto &solution : solutions) {

        if(function(index++, solution))break;
        
    }

}

std::ostream& operator<<(std::ostream& os, const _ga_population& obj) {
        
    obj.listConstSolutions([&os](unsigned index, const _ga_solution& solution){

//        const std::map<_ga_solution::EvalType, unsigned>& evals = solution.getEvals();
//        if(evals.empty()){
//
//            os << index << ": not evaluated" << std::endl;
//
//        }else{
//
//            os << index << ": " << evals.find(_ga_solution::EvalType::makespan)->second;
//            os << " / " << evals.find(_ga_solution::EvalType::energy)->second << std::endl;
//
//        }
        
        os << index << ":" << std::endl << solution << std::endl; 
        

        return false;

    });

    return os;
}