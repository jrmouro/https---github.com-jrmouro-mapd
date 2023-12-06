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
        
        _astarAlgorithm astar;
        _task_path task_path;

        unsigned makespan = 0, energy = 0;
        
        std::set<const _ga_agent*> currentAgents;
        
        std::map<const _ga_agent*, _path> agentPaths;
        std::vector<std::vector<const _site*>> paths(solution.allocation_map.size(), std::vector<const _site*>());
        std::vector<std::vector<_task_path::PathType>> types(solution.allocation_map.size(), std::vector<_task_path::PathType>());
        
        int index = 0;
        for (auto const& alloc_pair1 : solution.allocation_map) {
            
            _site initialSite;
            
            if(current){
                initialSite = alloc_pair1.first->currentSite();
            } else {
                initialSite = alloc_pair1.first->goalSite();
            }
            
            auto it = agentPaths.insert(
                std::pair<const _ga_agent*, _path>(
                    alloc_pair1.first, 
                    _path(initialSite)));
            
            if(!alloc_pair1.second.empty()){
                                    
                bool flag = astar.solve(token.getMap(), initialSite, alloc_pair1.second[0]->getPickup(), it.first->second);

                if(flag){
                    
                    it.first->second.rlist([&token, &paths, &types, &energy, index](const _site& site){
                        
                        paths[index].push_back(&site);
                        types[index].push_back(_task_path::PathType::pickup);
                        
                        energy += token.getAgent_energy_system().getMovingRegime();
                        
                        return false;
                        
                    });
                    
                } else {

                    try {
                        std::ostringstream stream;
                        stream << "path not found from " << alloc_pair1.first->goalSite() << " to " <<  alloc_pair1.second[0]->getPickup();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }
                
            } else {
                
                paths[index].push_back(&it.first->second.currentSite());
                types[index].push_back(_task_path::PathType::pickup);
                
                energy += token.getAgent_energy_system().getMovingRegime();
                
            }
                                    
            unsigned pickup_span, delivary_span;
            
            task_path.listSites(
                token.getMap().getEndpointsPathAlgorithm(),
                alloc_pair1.second, 
                1,
                pickup_span,
                delivary_span,
                [&token, &paths, &types, &energy, index](const _task_path::PathType& type, const _site& site, const _task* t1, const _task* t2){
                   
                    paths[index].push_back(&site);
                    types[index].push_back(type);
                    
                    if(type == _task_path::PathType::pickup){

                        energy += token.getAgent_energy_system().getMovingRegime();

                    } else {

                        energy += token.getAgent_energy_system().getCarringRegime();

                    }
                    
                    return false;
                    
                });
                
            makespan = std::max<unsigned>(paths[index].size(), makespan);  
            
            index++;
            
        }
        
        double pckp = .0, dlvy = .0, spanp = .0;
        
        for (int i = 0; i < paths.size(); i++) {
                        
            for (int j = i + 1; j < paths.size(); j++) {
                
                if(paths[i].size() > paths[j].size()){
                    
                    for (int k = 0; k < paths[j].size(); k++) {
                        
                        if(paths[j][k]->match(*paths[i][k])){
                            
                            spanp = spanp + makespan_penalty /*/ (k+1)*/;
                            
                            if(types[j][k] == _task_path::PathType::pickup){
                                
                                pckp = pckp + (pickup_energy_penalty /*/ (k+1)*/);
                                
                            } else {
                                
                                dlvy = dlvy + (delivery_energy_penalty /*/ (k+1)*/);
                                
                            }
                            
                        }

                    }
                    
                } else {
                    
                    for (int k = 0; k < paths[i].size(); k++) {
                        
                        if(paths[j][k]->match(*paths[i][k])){
                            
                            spanp = spanp + makespan_penalty /*/ (k+1)*/;
                            
                            if(types[j][k] == _task_path::PathType::pickup){
                                
                                pckp = pckp + (pickup_energy_penalty /*/ (k+1)*/);
                                
                            } else {
                                
                                dlvy = dlvy + (delivery_energy_penalty /*/ (k+1)*/);
                                
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