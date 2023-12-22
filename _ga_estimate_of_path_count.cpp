/* 
 * File:   _ga_estimate_of_path_count.cpp
 * Author: ronaldo
 * 
 * Created on 19 de novembro de 2023, 15:28
 */

#include "_ga_estimate_of_path_count.h"
#include "_endpointsPathAlgorithm.h"
#include "_task_path.h"


const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of_path_count::evals(const _ga_token& token, _ga_solution& solution) const{
    
    
    if(!solution.isEvaluated()){
        
        if(!solution.isAllocated()){
            
            solution.alloc(token);
            
        }
        
        unsigned makespan = 0, energy = 0;
        std::vector<std::vector<const _site*>> paths;
        std::vector<std::vector<_task_path::PathType>> types;
        std::map<const _ga_agent*, _path> agentPaths;
        
        if(current){
            
            pathSwap(
            token, 
            solution, 
            makespan,
            energy,
            agentPaths,
            paths,
            types);
            
        } else {
            
             path(
            token, 
            solution, 
            makespan,
            energy,
            agentPaths,
            paths,
            types);
            
            
        }
        
        double pckp = .0, dlvy = .0, spanp = .0;
        
        for (int i = 0; i < paths.size(); i++) {
                        
            for (int j = 0; j < paths.size(); j++) {
                
                if(i != j){
                    
                    for (int k = 0; k < paths[j].size(); k++) {

                        if(paths[j][k]->match(*paths[i][k])){

                            spanp = spanp + makespan_penalty;

                            if(types[i][k] == _task_path::PathType::pickup){

                                pckp = pckp + (pickup_energy_penalty);

                            } else {

                                dlvy = dlvy + (delivery_energy_penalty);

                            }

                        }

                    }
                
                }

            }

        }
        
        makespan += token.getCurrentStep()  + (unsigned)spanp;
        energy += token.energyExpenditure() + (unsigned)pckp + (unsigned)dlvy;
                    
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan));
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy));
                 
    }
    
    return solution.evals;
    
}