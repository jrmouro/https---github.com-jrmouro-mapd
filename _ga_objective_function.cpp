/* 
 * File:   _ga_objective_function.cpp
 * Author: ronaldo
 * 
 * Created on 14 de novembro de 2023, 16:17
 */

#include "_ga_objective_function.h"

unsigned _ga_objective_function::eval(const _ga_token& token, _ga_solution& solution, const _ga_solution::EvalType& evalType)const{
    
    const std::map<_ga_solution::EvalType, unsigned>& evals = this->evals(token, solution);

    std::map<_ga_solution::EvalType, unsigned>::const_iterator evalTypeIt = evals.find(evalType);

    if (evalTypeIt == evals.end()) {

        try {
            std::ostringstream stream;
            stream << "eval type not found: " << evalType;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return evalTypeIt->second;
    
}

