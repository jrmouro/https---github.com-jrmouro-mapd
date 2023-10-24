/* 
 * File:   _ga_solution_comparison.cpp
 * Author: ronaldo
 * 
 * Created on 19 de outubro de 2023, 15:25
 */

#include "_ga_solution_comparison.h"


_ga_solution_comparison::_ga_solution_comparison(_ga_solution::EvalType evalType) :
        evalType(evalType) {}

_ga_solution_comparison::_ga_solution_comparison(const _ga_solution_comparison& other) :
        evalType(other.evalType) {}

 _ga_solution_comparison::~_ga_solution_comparison(){}

bool _ga_solution_comparison::operator() (const _ga_solution& lhs, const _ga_solution&rhs) const {
    return lhs.evaluate(evalType) < rhs.evaluate(evalType);
}