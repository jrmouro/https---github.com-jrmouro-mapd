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

        unsigned makespan = 0;
        double energy = .0;
        std::map<const _ga_agent*, _stepPath> agentStepPaths;
        
        for (auto alloc_pair1 : solution.allocation_map) {
            
            if(!alloc_pair1.second.empty()){
                
                _path path;
                    
                bool flag = astar.solve(token.getMap(), alloc_pair1.first->goalSite(), alloc_pair1.second[0]->getPickup(), path);

                if(flag){
                    
                    _stepPath stepPath;
                    _stepPath::buildFromPath(alloc_pair1.first->goalSite().GetStep(), path, stepPath);
                    agentStepPaths.insert(std::pair<const _ga_agent*, _stepPath>(alloc_pair1.first, stepPath));
                    
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
                
                agentStepPaths.insert(std::pair<const _ga_agent*, _stepPath>(alloc_pair1.first, _stepPath(alloc_pair1.first->goalSite())));
                
            }
            
        }
        
        for( auto stepPath_pair1: agentStepPaths){
            
            unsigned currentStep1 = stepPath_pair1.second.goalSite().GetStep();  
            energy += (stepPath_pair1.second.size() * token.getAgent_energy_system().getMovingRegime());
            
            if(currentStep1 > makespan){

                makespan = currentStep1;

            }
            
            for( auto stepPath_pair2: agentStepPaths){

                if(stepPath_pair1.second.collideWith(stepPath_pair2.second)){

                    energy += token.getAgent_energy_system().getUnloadedRegime();

                }

            }
                       
            for (auto alloc_pair : solution.allocation_map){
                
                if(alloc_pair.first != stepPath_pair1.first){
                    
                    auto pathInitialPair1 = agentStepPaths.find(alloc_pair.first);                
                    unsigned currentStep2 = pathInitialPair1->second.goalSite().GetStep();
                    
                    task_path.listPaths(
                        token.getMap().getEndpointsPathAlgorithm(),
                        alloc_pair.second, 
                        [&solution, &stepPath_pair1, &token, &energy, &currentStep2](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){
                                                        
                            if(path2.size() > 0) {
                                
                                _stepPath stepPath2;
                                _stepPath::buildFromPath(currentStep2, path2, stepPath2);
                                currentStep2 = stepPath2.goalSite().GetStep();                            
                                                        
                                if(stepPath_pair1.second.goalSite().GetStep() < stepPath2.currentSite().GetStep()){

                                    return false;

                                } else if(stepPath_pair1.second.currentSite().GetStep() > stepPath2.goalSite().GetStep()){

                                    return true;

                                }
                                                                
                                unsigned count = stepPath_pair1.second.collideSiteCount(stepPath2);

                                if(type2 == _task_path::PathType::delivery){ // delivery

                                    energy += ((double)count) * token.getAgent_energy_system().getLoadedRegime();

                                } else { // pickup

                                    energy += ((double)count) * token.getAgent_energy_system().getUnloadedRegime();

                                } 
                            
                            }

                            return false;

                        });            
                    
                }
                                
            }
            
        }

        for (auto alloc_pair1 : solution.allocation_map) {

            if(!alloc_pair1.second.empty()){
                
                auto pathInitialPair1 = agentStepPaths.find(alloc_pair1.first);                
                unsigned currentStep1 = pathInitialPair1->second.goalSite().GetStep();   
                                                                
                task_path.listPaths(
                    token.getMap().getEndpointsPathAlgorithm(),
                    alloc_pair1.second, 
                    [&solution, &alloc_pair1, &currentStep1, &token, &energy, &makespan, &agentStepPaths, this](const _task_path::PathType& type1, const _path& path1, const _task* curr, const _task* next){
                                                
                        if(path1.size() > 0) {
                        
                            _stepPath stepPath1;
                            _stepPath::buildFromPath(currentStep1, path1, stepPath1);
                            currentStep1 = stepPath1.goalSite().GetStep();
                            
                            if(type1 == _task_path::PathType::delivery){ // delivery

                                energy += (stepPath1.size() * token.getAgent_energy_system().getCarringRegime());

                            } else { // pickup

                                energy += (stepPath1.size() * token.getAgent_energy_system().getMovingRegime());

                            } 

                            if(currentStep1 > makespan){

                                makespan = currentStep1;

                            }

                            for (auto alloc_pair2 : solution.allocation_map){

                                if(alloc_pair1.first != alloc_pair2.first){

                                    if(!alloc_pair2.second.empty()){

                                        auto pathInitialPair2 = agentStepPaths.find(alloc_pair2.first);                
                                        unsigned currentStep2 = pathInitialPair2->second.goalSite().GetStep(); 

                                        task_path.listPaths(
                                            token.getMap().getEndpointsPathAlgorithm(),
                                            alloc_pair2.second, 
                                            [&solution, &stepPath1, &currentStep2, &token, &energy, &type1](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){

                                                if(path2.size() > 0) {
                                                    
                                                    _stepPath stepPath2;
                                                    _stepPath::buildFromPath(currentStep2, path2, stepPath2);
                                                    currentStep2 = stepPath2.goalSite().GetStep();

                                                    if(stepPath1.goalSite().GetStep() < stepPath2.currentSite().GetStep()){
        //                                
                                                        return false;

                                                    } else if(stepPath1.currentSite().GetStep() > stepPath2.goalSite().GetStep()){

                                                        return true;

                                                    }

                                                    unsigned count = stepPath1.collideSiteCount(stepPath2);

                                                    if(type1 == _task_path::PathType::delivery){ // delivery

                                                        energy += ((double)count) * token.getAgent_energy_system().getLoadedRegime();

                                                    } else { // pickup

                                                        energy += ((double)count) * token.getAgent_energy_system().getUnloadedRegime();

                                                    } 

                                                }

                                                return false;

                                            });

                                    }

                                }

                            }
                        
                        }

                        return false;

                    });
                
            }
        }
            
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan));
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy));
                
    }
    
    return solution.evals;
    
}