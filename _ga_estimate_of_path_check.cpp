/* 
 * File:   _ga_estimate_of_path.cpp
 * Author: ronaldo
 * 
 * Created on 19 de novembro de 2023, 15:28
 */

#include "_ga_estimate_of_path.h"
#include "_endpointsPathAlgorithm.h"


const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of_path::evals(const _ga_token& token, _ga_solution& solution) const{
    
    if(!solution.isEvaluated()){
        
//        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    
        
        if(!solution.isAllocated()){
            
            solution.alloc(token);
            
        }
        
        _astarAlgorithm astar;

        unsigned makespan = 0;
        double energy = .0;
        std::map<const _ga_agent*, std::vector<_stepPath>> agentPaths;
        std::map<const _ga_agent*,std::vector<const _task*>> allocation_copy;

        for (auto alloc_pair : solution.allocation_map) {

            allocation_copy.insert(std::pair<const _ga_agent*,std::vector<const _task*>>(alloc_pair.first, alloc_pair.second));

            auto it = agentPaths.insert(std::pair<const _ga_agent*, std::vector<_stepPath>>(alloc_pair.first, std::vector<_stepPath>()));
                        
            _stepSite currentSite = it.first->first->currentSite();
            
            
            
            bool flag = true;
            for (auto task : alloc_pair.second) {
                
                _stepPath stepPath;

                if(true){
                    
                    _path path;
                    
                    flag = astar.solve(token.getMap(), currentSite, task->getPickup(), path);
                    
                    if(flag){
                                              
                        _stepPath::buildFromPath(currentSite.GetStep(),path, stepPath);
                        
                    } else {
                        
                        try {
                            std::ostringstream stream;
                            stream << "path not found from " << currentSite << " to " <<  task->getPickup();
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }
                        
                    }     
                    
                    flag = false;

                } else {
                    
                    _stepPath::buildFromPath(
                        currentSite.GetStep(), 
                        token.getMap().getEndpointsPathAlgorithm().solve_path(currentSite, task->getPickup()), 
                        stepPath);
                                   

                }
                
                it.first->second.push_back(stepPath);

                currentSite = stepPath.goalSite();  
                
                
                _stepPath::buildFromPath(
                    currentSite.GetStep(), 
                    token.getMap().getEndpointsPathAlgorithm().solve_path(task->getPickup(), task->getDelivery()), 
                    stepPath);
                        
                it.first->second.push_back(stepPath);

                currentSite = stepPath.goalSite();                      

            }

        }

        
        
        for (auto agentSites_pair1 : agentPaths) {
            
            if( !agentSites_pair1.second.empty() 
                && agentSites_pair1.second.back().goalSite().GetStep() > makespan){

                makespan = agentSites_pair1.second.back().goalSite().GetStep();

            }
            
            unsigned index = 0;
            
            for(const auto& path1: agentSites_pair1.second){
                
                for (auto agentSites_pair2 : agentPaths) {
                    
                    if(agentSites_pair1.first != agentSites_pair2.first){
                        
                        for(const auto& path2: agentSites_pair2.second){
                            
                            double energy_cost = .0;
                            
                            unsigned count = path1.collideCount(path2);
                            
//                            if(path1.collideWith(path2)){
            
                            if(index % 2 == 0){ // delivery

                                energy_cost = ((double)count) * token.getAgent_energy_system().getCarringRegime();

                            } else { // pickup

                                energy_cost = ((double)count) * token.getAgent_energy_system().getMovingRegime();

                            } 
                            
//                            }
                            
                            energy += energy_cost;
                        
                        }
                    
                    }

                }
                
                index++;
                
            }           
            
        }

        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan));
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy));

//        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
//        
//        std::cout << "_ga_estimate_of time span: " << time_span.count() << std::endl;
        
    }
    
    return solution.evals;
    
}