/* 
 * File:   _ga_select_solution.h
 * Author: ronaldo
 *
 * Created on 17 de novembro de 2023, 17:39
 */

#ifndef _GA_SELECT_SOLUTION_H
#define _GA_SELECT_SOLUTION_H

#include "_ga_solution.h"
#include "_ga_token.h"
#include "_ga_objective_function.h"
#include <vector>

class _ga_select_solution {
public:
    
    virtual _ga_solution* select(const _ga_token& token, const _ga_objective_function&, const std::vector<_ga_solution*>&) const = 0;

};

#endif /* _GA_SELECT_SOLUTION_H */

