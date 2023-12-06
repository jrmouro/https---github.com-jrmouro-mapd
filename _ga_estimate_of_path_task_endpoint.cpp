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
        
        _astarAlgorithm astar;
        _task_path task_path;

        unsigned makespan = 0,  energy = 0;
        
        std::set<const _ga_agent*> currentAgents;
        
        std::map<const _ga_agent*, unsigned> agentIndexes;
        std::map<const _ga_agent*, _path> agentPaths;
        std::vector<std::vector<const _site*>> paths(solution.allocation_map.size(), std::vector<const _site*>());
        
        std::vector<std::vector<unsigned>> pickupv(solution.allocation_map.size(), std::vector<unsigned>());
        std::vector<std::vector<unsigned>> deliveryv(solution.allocation_map.size(), std::vector<unsigned>());
        
        
        std::vector<std::vector<_task_path::PathType>> types(solution.allocation_map.size(), std::vector<_task_path::PathType>());
        
        int index = 0;
        for (auto const& alloc_pair1 : solution.allocation_map) {
            
            _site initialSite;
            
            if(current){
                initialSite = alloc_pair1.first->currentSite();
            } else {
                initialSite = alloc_pair1.first->goalSite();
            }
            
            agentIndexes.insert(std::pair<const _ga_agent*, unsigned>(alloc_pair1.first, index));
            
            auto it = agentPaths.insert(
                std::pair<const _ga_agent*, _path>(
                    alloc_pair1.first, 
                    _path(initialSite)));
            
            if(!alloc_pair1.second.empty()){
                                    
                bool flag = astar.solve(token.getMap(), initialSite, alloc_pair1.second[0]->getPickup(), it.first->second);

                if(flag){
                    
                    it.first->second.rlist([&token, &paths, &types, &pickupv, &deliveryv, &energy, index](const _site& site){
                        
                        paths[index].push_back(&site);
                        types[index].push_back(_task_path::PathType::pickup);
                        pickupv[index].push_back(0);
                        deliveryv[index].push_back(0);
                        
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
                pickupv[index].push_back(0);
                deliveryv[index].push_back(0);
                
                energy += token.getAgent_energy_system().getMovingRegime();
                
            }
            
            pickupv[index][pickupv[index].size() - 1] = pickup_energy_penalty;
            deliveryv[index][deliveryv[index].size() - 1] = pickup_energy_penalty;
                                    
            unsigned pickup_span, delivary_span;
            
            task_path.listSites(
                token.getMap().getEndpointsPathAlgorithm(),
                alloc_pair1.second, 
                1,
                pickup_span,
                delivary_span,
                [&token, &paths, &types, &pickupv, &deliveryv, &energy, index, this](const _task_path::PathType& type, const _site& site, const _task* t1, const _task* t2){
                   
                    paths[index].push_back(&site);
                    types[index].push_back(type);
                    
                    if(site.match(t1->getPickup())){
                        pickupv[index].push_back(pickup_energy_penalty);
                    } else {
                        pickupv[index].push_back(0);
                    }
                    
                    if(site.match(t1->getDelivery())){
                        deliveryv[index].push_back(delivery_energy_penalty);
                    } else {
                        deliveryv[index].push_back(0);
                    }
                                        
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
                        
            for (int j = 0; j < paths.size(); j++) {
                
                if(i != j){
                
                    if(paths[i].size() > paths[j].size()){

                        for (int k = 0; k < paths[j].size(); k++) {
                                                        
                            if(paths[j][k]->match(*paths[i][k])){
                                
                                if(pickupv[j][k] != 0 || deliveryv[j][k] != 0){

                                    spanp = spanp + makespan_penalty  /*/ (k+1)*/;
                                
                                }

                                if(types[j][k] == _task_path::PathType::pickup){

                                    pckp = pckp + (pickupv[j][k]  /*/ (k+1)*/);

                                } else {

                                    dlvy = dlvy + (deliveryv[j][k] /*/ (k+1)*/);

                                }

                            }

                        }

                    } else {

                        for (int k = 0; k < paths[i].size(); k++) {

                            if(paths[j][k]->match(*paths[i][k])){

                                if(pickupv[j][k] != 0 || deliveryv[j][k] != 0){

                                    spanp = spanp + makespan_penalty /*/ (k+1)*/;
                                
                                }

                                if(types[j][k] == _task_path::PathType::pickup){

                                    pckp = pckp + (pickupv[j][k]  /*/ (k+1)*/);

                                } else {

                                    dlvy = dlvy + (deliveryv[j][k] /*/ (k+1)*/);

                                }

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