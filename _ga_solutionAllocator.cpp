/* 
 * File:   _ga_solutionAllocator.cpp
 * Author: ronaldo
 * 
 * Created on 26 de outubro de 2023, 10:07
 */

#include "_ga_solutionAllocator.h"
#include "_ga_solution.h"

_ga_solutionAllocator::_ga_solutionAllocator(
    unsigned solution_validity, 
    const std::string id) : _agentsTasksAllocator(id),
        solution_validity(solution_validity) { }
    
_ga_solutionAllocator::_ga_solutionAllocator(
    const _ga_solutionAllocator& other) : 
        _agentsTasksAllocator(other),
        solution_validity(other.solution_validity){ }
    

_ga_solutionAllocator::~_ga_solutionAllocator(){
    
    for (auto solution : borrowed) {

        delete solution;
        
    }

}

_allocation* _ga_solutionAllocator::restore(const _ga_token& token, _allocation* allocation){
    
    ((_ga_solution*)allocation)->restore(token);
    
    return allocation;
    
}
    
_allocation* _ga_solutionAllocator::borrow(const _ga_token& token) {
    
    _ga_solution* best = new _ga_solution(token, solution_validity);
    
//    ret->evaluate(token);
    
    borrowed.insert(best);
    
//    best->evaluate(token);
//    std::cout << "solution: " << *best << std::endl;
    
    return best;
    
}

void _ga_solutionAllocator::giveBack(_allocation* allocation) {
    
    std::set<_ga_solution*>::iterator it = borrowed.find((_ga_solution*)allocation);
    
    if(it != borrowed.end()){
        
        _ga_solution* solution = *it;
                
        borrowed.erase(it);
        
        delete solution;
        
    } else {
        
        try {
            std::ostringstream stream;
            stream << "unrecognized allocation";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
}

_agentsTasksAllocator* _ga_solutionAllocator::emptyClone() const {
    return new _ga_solutionAllocator(*this);
}

_allocation* _ga_solutionAllocator::borrowClone(_allocation* allocation) {
    
    std::set<_ga_solution*>::iterator it = borrowed.find((_ga_solution*)allocation);
    
    if(it != borrowed.end()){
        
        _ga_solution* solution = (_ga_solution*)(*it)->clone();   
        
        borrowed.insert(solution);
                
        return solution;
        
    } else {
        
        try {
            std::ostringstream stream;
            stream << "unrecognized allocation";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }  
    
    return nullptr;
    
}