/* 
 * File:   _ga_estimate_of_path_check.cpp
 * Author: ronaldo
 * 
 * Created on 19 de novembro de 2023, 15:28
 */

#include "_ga_estimate_of_path_check.h"
#include "_endpointsPathAlgorithm.h"
#include "_task_path.h"


const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of_path_check::evals(const _ga_token& token, _ga_solution& solution) const{
    
    if(!solution.isEvaluated()){
        
        if(!solution.isAllocated()){
            
            solution.alloc(token);
            
        }
        
        _astarAlgorithm astar;

        unsigned 
                makespan = 0, 
                msp_penalty = 0, 
                energy = 0, 
                eng_penalty = 0;
        
        std::set<const _ga_agent*> currentAgents;
        
        std::map<const _ga_agent*, _path> agentPaths;
        std::vector<std::vector<const _site*>> paths(solution.allocation_map.size(), std::vector<const _site*>());
        std::vector<std::vector<_task_path::PathType>> types(solution.allocation_map.size(), std::vector<_task_path::PathType>());
        
        int index = 0;
        for (auto const& alloc_pair1 : solution.allocation_map) {
            
            auto it = agentPaths.insert(
                std::pair<const _ga_agent*, _path>(
                    alloc_pair1.first, 
                    _path(_site(
                        alloc_pair1.first->goalSite().GetRow(), 
                        alloc_pair1.first->goalSite().GetColunm()))));
            
            if(!alloc_pair1.second.empty()){
                                    
                bool flag = astar.solve(token.getMap(), alloc_pair1.first->goalSite(), alloc_pair1.second[0]->getPickup(), it.first->second);

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
                0,
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
        
        unsigned pckp = 0, dlvy = 0;
        
        for (int i = 0; i < paths.size(); i++) {
                        
            for (int j = i + 1; j < paths.size(); j++) {
                
                if(paths[i].size() > paths[j].size()){
                    
                    for (int k = 0; k < paths[j].size(); k++) {
                        
                        if(paths[j][k]->match(*paths[i][k])){
                            
                            if(types[j][k] == _task_path::PathType::pickup){
                                
                                pckp++;
                                
                            } else {
                                
                                dlvy++;
                                
                            }
                            
                            break;
                            
                        }

                    }
                    
                } else {
                    
                    for (int k = 0; k < paths[i].size(); k++) {
                        
                        if(paths[j][k]->match(*paths[i][k])){
                            
                            if(types[j][k] == _task_path::PathType::pickup){
                                
                                pckp++;
                                
                            } else {
                                
                                dlvy++;
                                
                            }
                            
                            break;
                            
                        }

                    }                    
                    
                }

            }

        }
        
        eng_penalty += (pckp * pickup_energy_penalty) + (dlvy * delivery_energy_penalty);

        msp_penalty += (pckp + dlvy) * makespan_penalty;
                                
                    
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan + msp_penalty));
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy + eng_penalty));
                
    }
    
    return solution.evals;
    
    
//    if(!solution.isEvaluated()){
//        
//        if(!solution.isAllocated()){
//            
//            solution.alloc(token);
//            
//        }
//        
//        _astarAlgorithm astar;
//
//        unsigned 
//                makespan = 0, 
//                msp_penalty = 0, 
//                energy = 0, 
//                eng_penalty = 0;
//        
//        std::set<const _ga_agent*> currentAgents;
//        
//        std::map<const _ga_agent*, _stepPath> agentStepPaths;
//        
//        for (auto const& alloc_pair1 : solution.allocation_map) {
//            
//            if(!alloc_pair1.second.empty()){
//                
//                _path path;
//                    
//                bool flag = astar.solve(token.getMap(), alloc_pair1.first->goalSite(), alloc_pair1.second[0]->getPickup(), path);
//
//                if(flag){
//                    
//                    _stepPath stepPath;
//                    _stepPath::buildFromPath(alloc_pair1.first->goalSite().GetStep(), path, stepPath);
//                    agentStepPaths.insert(std::pair<const _ga_agent*, _stepPath>(alloc_pair1.first, stepPath));
//                    
//                } else {
//
//                    try {
//                        std::ostringstream stream;
//                        stream << "path not found from " << alloc_pair1.first->goalSite() << " to " <<  alloc_pair1.second[0]->getPickup();
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }
//
//                }
//                
//            } else {
//                
//                agentStepPaths.insert(std::pair<const _ga_agent*, _stepPath>(alloc_pair1.first, _stepPath(alloc_pair1.first->goalSite())));
//                
//            }
//            
//        }
//        
//        for( auto const& stepPath_pair1: agentStepPaths){
//            
//            currentAgents.insert(stepPath_pair1.first);
//            
//            unsigned currentStep1 = stepPath_pair1.second.goalSite().GetStep();  
//            energy += (stepPath_pair1.second.size() * token.getAgent_energy_system().getMovingRegime());
//            
//            if(currentStep1 > makespan){
//
//                makespan = currentStep1;
//
//            }
//            
//            for( auto const& stepPath_pair2: agentStepPaths){
//                
//                if(currentAgents.find(stepPath_pair2.first) == currentAgents.end()){
//                
////                if(stepPath_pair2.first != stepPath_pair1.first){
//                    
//                    if(stepPath_pair1.second.collideSiteWith(stepPath_pair2.second)){
//                        
//                        eng_penalty += 2 * pickup_energy_penalty;
//                        msp_penalty += makespan_penalty;
//                        
//                    }
//                    
//                }
//
//            }
//                       
//            for (auto const& alloc_pair : solution.allocation_map){
//                
//                if(currentAgents.find(alloc_pair.first) == currentAgents.end()){
//                
////                if(alloc_pair.first != stepPath_pair1.first){
//                    
//                    auto pathInitialPair1 = agentStepPaths.find(alloc_pair.first);                
//                    unsigned currentStep2 = pathInitialPair1->second.goalSite().GetStep();
//                    
//                    task_path.listPaths(
//                        token.getMap().getEndpointsPathAlgorithm(),
//                        alloc_pair.second, 
//                        [&solution, &stepPath_pair1, &token, &makespan, &msp_penalty, &eng_penalty, &energy, &currentStep2, this](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){
//                                                        
//                            if(path2.size() > 0) {
//                                
//                                _stepPath stepPath2;
//                                _stepPath::buildFromPath(currentStep2, path2, stepPath2);
//                                currentStep2 = stepPath2.goalSite().GetStep();  
//                                
//                                if(currentStep2 > makespan){
//
//                                    makespan = currentStep2;
//
//                                }
//                                                        
//                                if(stepPath_pair1.second.goalSite().GetStep() < stepPath2.currentSite().GetStep()){
//
//                                    return false;
//
//                                } else if(stepPath_pair1.second.currentSite().GetStep() > stepPath2.goalSite().GetStep()){
//
//                                    return true;
//
//                                }
//                                                                                                
//                                if(stepPath_pair1.second.collideSiteWith(stepPath2)){
//                                    
//                                    if(type2 == _task_path::PathType::delivery){ // delivery
//
//                                        eng_penalty += delivery_energy_penalty;
//
//                                    } else { // pickup
//
//                                        eng_penalty += pickup_energy_penalty;
//
//                                    } 
//
//                                    eng_penalty += pickup_energy_penalty;
//                                    msp_penalty += makespan_penalty;
//                                    
//                                }
//                                
//                            }
//
//                            return false;
//
//                        });            
//                    
//                }
//                                
//            }
//            
//        }
//        
//        currentAgents.clear();
//
//        for (auto const& alloc_pair1 : solution.allocation_map) {
//            
//            currentAgents.insert(alloc_pair1.first);
//
//            if(!alloc_pair1.second.empty()){
//                
//                auto pathInitialPair1 = agentStepPaths.find(alloc_pair1.first);                
//                unsigned currentStep1 = pathInitialPair1->second.goalSite().GetStep();   
//                                                                
//                task_path.listPaths(
//                    token.getMap().getEndpointsPathAlgorithm(),
//                    alloc_pair1.second, 
//                    [&solution, &alloc_pair1, &currentStep1, &token, &energy, &makespan, &msp_penalty, &eng_penalty, &agentStepPaths, &currentAgents, this](const _task_path::PathType& type1, const _path& path1, const _task* curr, const _task* next){
//                                                
//                        if(path1.size() > 0) {
//                        
//                            _stepPath stepPath1;
//                            _stepPath::buildFromPath(currentStep1, path1, stepPath1);
//                            currentStep1 = stepPath1.goalSite().GetStep();
//                            
//                            if(type1 == _task_path::PathType::delivery){ // delivery
//
//                                energy += (stepPath1.size() * token.getAgent_energy_system().getCarringRegime());
//
//                            } else { // pickup
//
//                                energy += (stepPath1.size() * token.getAgent_energy_system().getMovingRegime());
//
//                            } 
//
//                            if(currentStep1 > makespan){
//
//                                makespan = currentStep1;
//
//                            }
//
//                            for (auto const& alloc_pair2 : solution.allocation_map){
//                                
//                                if(currentAgents.find(alloc_pair2.first) == currentAgents.end()){
//
////                                if(alloc_pair1.first != alloc_pair2.first){
//
//                                    if(!alloc_pair2.second.empty()){
//
//                                        auto pathInitialPair2 = agentStepPaths.find(alloc_pair2.first);                
//                                        unsigned currentStep2 = pathInitialPair2->second.goalSite().GetStep(); 
//
//                                        task_path.listPaths(
//                                            token.getMap().getEndpointsPathAlgorithm(),
//                                            alloc_pair2.second, 
//                                            [&solution, &stepPath1, &currentStep2, &token, &makespan, &msp_penalty, &eng_penalty, &energy, &type1, this](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){
//
//                                                if(path2.size() > 0) {
//                                                    
//                                                    _stepPath stepPath2;
//                                                    _stepPath::buildFromPath(currentStep2, path2, stepPath2);
//                                                    currentStep2 = stepPath2.goalSite().GetStep();
//                                                    
//                                                    if(currentStep2 > makespan){
//
//                                                        makespan = currentStep2;
//
//                                                    }
//
//                                                    if(stepPath1.goalSite().GetStep() < stepPath2.currentSite().GetStep()){
//        //                                
//                                                        return false;
//
//                                                    } else if(stepPath1.currentSite().GetStep() > stepPath2.goalSite().GetStep()){
//
//                                                        return true;
//
//                                                    }
//                                                    
//                                                    if(stepPath1.collideSiteWith(stepPath2)){
//                                    
//                                                        if(type1 == _task_path::PathType::delivery){ // delivery
//
//                                                            eng_penalty += delivery_energy_penalty;
//
//                                                        } else { // pickup
//
//                                                            eng_penalty += pickup_energy_penalty;
//
//                                                        } 
//                                                        
//                                                        if(type2 == _task_path::PathType::delivery){ // delivery
//
//                                                            eng_penalty += delivery_energy_penalty;
//
//                                                        } else { // pickup
//
//                                                            eng_penalty += pickup_energy_penalty;
//
//                                                        }
//
//                                                        msp_penalty += makespan_penalty;
//
//                                                    }    
//
//                                                }
//
//                                                return false;
//
//                                            });
//
//                                    }
//
//                                }
//
//                            }
//                        
//                        }
//
//                        return false;
//
//                    });
//                
//            }
//        }
//            
//        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan + msp_penalty));
//        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy + eng_penalty));
//                
//    }
//    
//    return solution.evals;
    
}