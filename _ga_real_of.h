/* 
 * File:   _ga_real_of.h
 * Author: ronaldo
 *
 * Created on 14 de novembro de 2023, 16:04
 */

#ifndef _GA_REAL_OF_H
#define _GA_REAL_OF_H

#include "_ga_objective_function.h"

class _ga_real_of : public _ga_objective_function {
    
public:
    
    virtual const std::map<_ga_solution::EvalType, unsigned>& evals(const _ga_token&, _ga_solution&) const;
    
private:

};

#endif /* _GA_REAL_OF_H */

