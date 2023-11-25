/* 
 * File:   _ga_best_makespan_selector.cpp
 * Author: ronaldo
 * 
 * Created on 20 de novembro de 2023, 15:23
 */

#include "_ga_best_solution_selector.h"
#include "_ga_balanced_solution.h"
#include <limits>

_ga_best_solution_selector::_ga_best_solution_selector(_ga_solution::EvalType evaltype) :
    evaltype(evaltype) { }
    
_ga_best_solution_selector::_ga_best_solution_selector(const _ga_best_solution_selector& other) :
evaltype(other.evaltype) { }

_ga_best_solution_selector::~_ga_best_solution_selector(){}

_ga_solution* _ga_best_solution_selector::select(
        const _ga_token& token, 
        const _ga_objective_function& objective_function, 
        const std::vector<_ga_solution*>& solutions) const {
    
    switch(evaltype){
        case _ga_solution::EvalType::makespan:
            return select_makespan(token, objective_function, solutions);
        case _ga_solution::EvalType::energy:
            return select_energy(token, objective_function, solutions);
    }
    
    return balanced_solution.select(token, objective_function, solutions);
    
}

_ga_solution* _ga_best_solution_selector::select_makespan(
        const _ga_token& token, 
        const _ga_objective_function& objective_function, 
        const std::vector<_ga_solution*>& solutions) const{
    
    _ga_solution* ret = nullptr;    
    unsigned mksp = UINT_MAX;
        
    for(int i = 0; i < solutions.size(); i++){
        
        _ga_solution* solution = solutions[i];
                            
        unsigned aux = (float)objective_function.eval(token, *solution, _ga_solution::EvalType::makespan);
                
        if(aux < mksp){
            mksp = aux;
            ret = solution;
        }
        
    }    
    
    return ret;
    
}

_ga_solution* _ga_best_solution_selector::select_energy(
        const _ga_token& token, 
        const _ga_objective_function& objective_function, 
        const std::vector<_ga_solution*>& solutions) const{
    
    _ga_solution* ret = nullptr;    
    unsigned engy = UINT_MAX;
        
    for(int i = 0; i < solutions.size(); i++){
        
        _ga_solution* solution = solutions[i];
                            
        unsigned aux = (float)objective_function.eval(token, *solution, _ga_solution::EvalType::energy);
                
        if(aux < engy){
            engy = aux;
            ret = solution;
        }
        
    }    
    
    return ret;
    
}