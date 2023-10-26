/* 
 * File:   _ga_solutionAllocator.cpp
 * Author: ronaldo
 * 
 * Created on 26 de outubro de 2023, 10:07
 */

#include "_ga_solutionAllocator.h"
#include "_ga_solution.h"


_allocation* _ga_solutionAllocator::solve(const _ga_token& token) const{
    
    return new _ga_solution(token);
    
}