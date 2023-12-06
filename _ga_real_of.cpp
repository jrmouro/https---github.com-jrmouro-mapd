/* 
 * File:   _ga_real_of.cpp
 * Author: ronaldo
 * 
 * Created on 14 de novembro de 2023, 16:04
 */

#include "_ga_real_of.h"

#include <ratio>
#include <chrono>

const std::map<_ga_solution::EvalType, unsigned>& _ga_real_of::evals(const _ga_token& token, _ga_solution& solution) const{
    
    if(!solution.isEvaluated()){
        
//        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
                
        if(!solution.isAllocated()){
            
            solution.alloc(token);
            
        }
        
        _ga_token* tokenAux = token.getClone();
        _ga_solution solutionAux(solution);
        
               

        unsigned count = 0;
        
        unsigned validity = solution.validity();

        while ( //count < validity && 
                tokenAux->getCurrentStep() < tokenAux->getStepMap().getStep_size() && !tokenAux->isIdle()) {

//                        tokenAux.error_site_collision_check(); // TODO retirar
//                        tokenAux.error_edge_collision_check();


            solutionAux.nextPlanningUpdate(*tokenAux, [tokenAux](const _ga_agent* agent, const _task * task) {

                if (task != nullptr) {

                    return tokenAux->updateAgentTaskPath(agent->id(), task->id());

                } else {

                    return tokenAux->updateAgentTaskPath(agent->id(), INT_MIN);

                }

            });
            
//            if(tokenAux.isIdle()) break;

            tokenAux->stepping();
//            count++;

        }
        
        

        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, tokenAux->GetMaxPlannedStep()));
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, tokenAux->energyExpenditure()));
        
        delete tokenAux;
        
//        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
//        
//        std::cout << "_ga_real_of time span: " << time_span.count() << std::endl;
        
    }
    
    return solution.evals;
    
}
