/* 
 * File:   _ga_balanced_solution.h
 * Author: ronaldo
 *
 * Created on 17 de novembro de 2023, 17:44
 */

#ifndef _GA_BALANCED_SOLUTION_H
#define _GA_BALANCED_SOLUTION_H

#include "_ga_select_solution.h"
#include "_ga_objective_function.h"

class _ga_balanced_solution : public _ga_select_solution{
    
public:
        
    virtual _ga_solution* select(const _ga_token& , const _ga_objective_function&, const std::vector<_ga_solution*>&) const;
        
};

#endif /* _GA_BALANCED_SOLUTION_H */

