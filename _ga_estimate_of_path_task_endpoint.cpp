/* 
 * File:   _ga_estimate_of_path_task_endpoint.cpp
 * Author: ronaldo
 * 
 * Created on 1 de dezembro de 2023, 06:57
 */

#include "_ga_estimate_of_path_task_endpoint.h"
#include "_endpointsPathAlgorithm.h"
#include "_task_path.h"

const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of_path_task_endpoint::evals(const _ga_token& token, _ga_solution& solution) const{
    
    
    if(!solution.isEvaluated()){
        
        if(!solution.isAllocated()){
            
            solution.alloc(token);
            
        }
        
        unsigned makespan = 0, energy = 0;
        std::vector<std::vector<const _site*>> paths;
        std::vector<std::vector<_task_path::PathType>> types;
        std::map<const _ga_agent*, _path> agentPaths;
        std::vector<std::vector<unsigned>> pickupv;
        std::vector<std::vector<unsigned>> deliveryv;
        
        if(current){
            
            pathSwapTaskEndpoint(
            token, 
            solution, 
            makespan,
            energy,
            agentPaths,
            paths,
            types,
            pickupv, 
            deliveryv);
            
        } else {
            
             pathTaskEndpoint(
            token, 
            solution, 
            makespan,
            energy,
            agentPaths,
            paths,
            types,
            pickupv, 
            deliveryv);
            
            
        }
                        
        double pckp = .0, dlvy = .0, spanp = .0;
                        
        for (int i = 0; i < paths.size(); i++) {
                        
            for (int j = 0; j < paths.size(); j++) {
                
                if(i != j){
                
                    for (int k = 0; k < paths[j].size(); k++) {

                        if(paths[j][k]->match(*paths[i][k])){

                            if(pickupv[j][k] != 0 || deliveryv[j][k] != 0){

                                spanp = spanp + makespan_penalty;

                            }

                            if(types[i][k] == _task_path::PathType::pickup){

                                pckp = pckp + (pickupv[j][k]);

                            } else {

                                dlvy = dlvy + (deliveryv[j][k]);

                            }

                        }

                    }
                
                }

            }

        }
                
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan + (unsigned)spanp));
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy + (unsigned)pckp + (unsigned)dlvy));
                
    }
    
    return solution.evals;
    
}