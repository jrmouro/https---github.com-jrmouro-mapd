/* 
 * File:   _ga_estimate_of_path_collision.cpp
 * Author: ronaldo
 * 
 * Created on 19 de novembro de 2023, 15:28
 */

#include "_ga_estimate_of_path_collision.h"
#include "_endpointsPathAlgorithm.h"
#include "_task_path.h"


const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of_path_collision::evals(const _ga_token& token, _ga_solution& solution) const{
    
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
            
            for (int j = i; j < paths.size(); j++) {
                
                if(j != i){
                                
                    for (int k = 0; k < paths[j].size(); k++) {

                        bool pflag = false, dflag = false;

                        paths[j][k]->listNeighbors([i, j, k, &dlvy, &pckp, &spanp, &paths, &types, &dflag, &pflag, this](const _site& neight){

                            if(paths[i][k]->match(neight)){

                                spanp = spanp + makespan_penalty;

                                if(types[j][k] == _task_path::PathType::pickup && !pflag){

                                    pckp = pckp + (pickup_energy_penalty);
                                    pflag = true;

                                } else if(!dflag){

                                    dlvy = dlvy + (delivery_energy_penalty);
                                    dflag = true;

                                }  

                                if(types[i][k] == _task_path::PathType::pickup && !pflag){

                                    pckp = pckp + (pickup_energy_penalty);
                                    pflag = true;

                                } else if(!dflag){

                                    dlvy = dlvy + (delivery_energy_penalty);
                                    dflag = true;

                                }  

                            }

                            return dflag || pflag;

                        });

                        if(dflag || pflag) break;

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