/* 
 * File:   _ga_balanced_solution.cpp
 * Author: ronaldo
 * 
 * Created on 17 de novembro de 2023, 17:44
 */

#include "_ga_balanced_solution.h"
#include <limits>

_ga_solution* _ga_balanced_solution::select(
        const _ga_token& token, 
        const _ga_objective_function& objective_function, 
        const std::vector<_ga_solution*>& solutions) const {
    
    _ga_solution* ret = nullptr;
    
    unsigned size = solutions.size();
    
    int index = -1;
        
    std::vector<float> mspv(size, .0f), engv(size, .0f);
    
    float max_msp = .0f, max_eng = .0f, min_dist = std::numeric_limits<float>::min();    
    
    for(int i = 0; i < solutions.size(); i++){
        
        _ga_solution* solution = solutions[i];
                            
        mspv[i] = (float)objective_function.eval(token, *solution, _ga_solution::EvalType::makespan);
        engv[i] = (float)objective_function.eval(token, *solution, _ga_solution::EvalType::energy);
                
        if(mspv[i] > max_msp) max_msp = mspv[i];
        if(engv[i] > max_eng) max_eng = engv[i];
        
    }
    
    for(int i = 0; i < solutions.size(); i++){
        
        if(max_msp > .0f) mspv[i] = mspv[i] / max_msp;
        if(max_eng > .0f) engv[i] = engv[i] / max_eng;
        
        float dist = (mspv[i] * mspv[i]) + (engv[i] * engv[i]);
        
        if(dist < min_dist) min_dist = dist;
        index = i;
                
    }
    
    if(index != -1) ret = solutions[index];

    return ret;
    
}