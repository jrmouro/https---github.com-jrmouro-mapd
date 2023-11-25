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
        
        _astarAlgorithm astar;
        
        unsigned validity = token.getCurrentStep() + solution.validity();

        unsigned makespan = 0;
        double energy = .0;
        std::map<const _ga_agent*, _path> agentPaths;
        
        for (auto const& alloc_pair1 : solution.allocation_map) {
            
            if(!alloc_pair1.second.empty()){
                
                _path path;
                    
                bool flag = astar.solve(token.getMap(), alloc_pair1.first->goalSite(), alloc_pair1.second[0]->getPickup(), path);

                if(flag){
                    
                    agentPaths.insert(std::pair<const _ga_agent*, _path>(alloc_pair1.first, path));
                    
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
                
                agentPaths.insert(std::pair<const _ga_agent*, _path>(alloc_pair1.first, _path(alloc_pair1.first->goalSite())));
                
            }
            
        }
        
        for(auto const& path_pair1: agentPaths){
            
            unsigned currentStep1 = path_pair1.first->goalSite().GetStep();  
            energy += (path_pair1.second.size() * token.getAgent_energy_system().getUnloadedRegime());
            
            if(currentStep1 > makespan){

                makespan = currentStep1;

            }
            
//            unsigned validity1 = currentStep1 + solution.getValidity();
            
            path_pair1.second.rlistPortion(task_path.getPortion(), [&token, &energy, &solution, &agentPaths, &path_pair1, &currentStep1, validity, this](const _path& path1){
                
                if(validity > currentStep1){
                
                    _stepPath stepPath1;
                    _stepPath::buildFromPath(currentStep1, path1, stepPath1);
                    currentStep1 = stepPath1.goalSite().GetStep();

    //                std::cout << "test: " << std::endl << stepPath1 << std::endl;

                    for(const auto& path_pair2: agentPaths){

                        if(path_pair1.first != path_pair2.first){

                            unsigned currentStep2 = path_pair2.first->goalSite().GetStep(); 
//                            unsigned validity2 = currentStep2 + solution.getValidity();

                            path_pair2.second.rlistPortion(task_path.getPortion(), [&token, &solution, &energy, &path_pair2, &stepPath1, validity, &currentStep2](const _path& path2){

                                if(validity > currentStep2){
                                
                                    _stepPath stepPath2;
                                    _stepPath::buildFromPath(currentStep2, path2, stepPath2);
                                    currentStep2 = stepPath2.goalSite().GetStep();

                                    if(stepPath1.collideWith(stepPath2)){

                                        energy += token.getAgent_energy_system().getUnloadedRegime();

                                    }
                                
                                } else {
                                    
                                    return true;
                                    
                                }

                                return false;

                            });

                        }                   

                    }

                    for (auto const& alloc_pair : solution.allocation_map){

                        if(alloc_pair.first != path_pair1.first){

                            unsigned currentStep2 = alloc_pair.first->goalSite().GetStep() + agentPaths.find(alloc_pair.first)->second.size() - 1;
//                            unsigned validity2 = currentStep2 + solution.getValidity();
                            
                            task_path.listPortionPaths(
                                token.getMap().getEndpointsPathAlgorithm(),
                                alloc_pair.second, 
                                [&solution, &path_pair1, &alloc_pair, &token, &energy, validity, &currentStep2, &stepPath1](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){

                                    if(validity > currentStep2){
                                    
                                        if(path2.size() > 0) {

                                            _stepPath stepPath2;
                                            _stepPath::buildFromPath(currentStep2, path2, stepPath2);
                                            currentStep2 = stepPath2.goalSite().GetStep();  

                                            if(stepPath1.goalSite().GetStep() < stepPath2.currentSite().GetStep()){

                                                return false;

                                            } else if(stepPath1.currentSite().GetStep() > stepPath2.goalSite().GetStep()){

                                                return true;

                                            }

                                            if(stepPath1.collideWith(stepPath2)){

                                                energy += token.getAgent_energy_system().getUnloadedRegime();

                                            } 

                                        }
                                    
                                    } else {
                                        
                                        return true;
                                        
                                    }

                                    return false;

                                });            

                        }

                    }
                
                } else {
                    
                    return true;
                    
                }
                       
                return false;
                
            });
            
        }
//
        for (auto const& alloc_pair1 : solution.allocation_map) {

            if(!alloc_pair1.second.empty()){
                                
                unsigned currentStep1 = alloc_pair1.first->goalSite().GetStep() + agentPaths.find(alloc_pair1.first)->second.size() - 1;  
//                unsigned validity1 = currentStep1 + solution.getValidity();
                                                                
                task_path.listPortionPaths(
                    token.getMap().getEndpointsPathAlgorithm(),
                    alloc_pair1.second, 
                    [&solution, &alloc_pair1, validity, &currentStep1, &token, &energy, &makespan, &agentPaths, this](const _task_path::PathType& type1, const _path& path1, const _task* curr, const _task* next){
                                              
                        if(validity > currentStep1){
                        
                            if(path1.size() > 0) {

                                _stepPath stepPath1;
                                _stepPath::buildFromPath(currentStep1, path1, stepPath1);
                                currentStep1 = stepPath1.goalSite().GetStep();

                                if(type1 == _task_path::PathType::delivery){ // delivery

                                    energy += (stepPath1.size() * token.getAgent_energy_system().getLoadedRegime());

                                } else { // pickup

                                    energy += (stepPath1.size() * token.getAgent_energy_system().getUnloadedRegime());

                                }                            

                                if(currentStep1 > makespan){

                                    makespan = currentStep1;

                                }

                                for (auto const& alloc_pair2 : solution.allocation_map){

                                    if(alloc_pair1.first != alloc_pair2.first){

                                        if(!alloc_pair2.second.empty()){

                                            unsigned currentStep2 = alloc_pair2.first->goalSite().GetStep() + agentPaths.find(alloc_pair2.first)->second.size() - 1;  
//                                            unsigned validity2 = currentStep2 + solution.getValidity();

                                            task_path.listPortionPaths(
                                                token.getMap().getEndpointsPathAlgorithm(),
                                                alloc_pair2.second, 
                                                [&solution, &stepPath1, validity, &currentStep2, &token, &energy, &type1](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){

                                                    if(validity > currentStep2){
                                                    
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

                                                            if(stepPath1.collideWith(stepPath2)){

                                                                if(type1 == _task_path::PathType::delivery){ // delivery

                                                                    energy += token.getAgent_energy_system().getLoadedRegime();

                                                                } else { // pickup

                                                                    energy += token.getAgent_energy_system().getUnloadedRegime();

                                                                } 

                                                            }

                                                        }
                                                    
                                                    } else {
                                                        
                                                        return true;
                                                        
                                                    }

                                                    return false;

                                                });

                                        }

                                    }

                                }

                            } else {

                                try {
                                    std::ostringstream stream;
                                    stream << "empty path";
                                    MAPD_EXCEPTION(stream.str());
                                } catch (std::exception& e) {
                                    std::cout << e.what() << std::endl;
                                    std::abort();
                                }

                            }
                        
                        } else {
                            
                            return true;
                            
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

//const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of_path_collision::evals(const _ga_token& token, _ga_solution& solution) const{
//    
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
//        unsigned makespan = 0;
//        double energy = .0;
//        std::map<const _ga_agent*, _stepPath> agentStepPaths;
//        
//        for (auto alloc_pair1 : solution.allocation_map) {
//            
//            if(!alloc_pair1.second.empty()){
//                
//                _path path;
//                    
//                bool flag = astar.solve(token.getMap(), alloc_pair1.first->currentSite(), alloc_pair1.second[0]->getPickup(), path);
//
//                if(flag){
//                    
//                    _stepPath stepPath;
//                    _stepPath::buildFromPath(alloc_pair1.first->currentSite().GetStep(), path, stepPath);
//                    agentStepPaths.insert(std::pair<const _ga_agent*, _stepPath>(alloc_pair1.first, stepPath));
//                    
//                } else {
//
//                    try {
//                        std::ostringstream stream;
//                        stream << "path not found from " << alloc_pair1.first->currentSite() << " to " <<  alloc_pair1.second[0]->getPickup();
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }
//
//                }
//                
//            }
//            
//        }
//        
//        for( auto stepPath_pair1: agentStepPaths){
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
//            for( auto stepPath_pair2: agentStepPaths){
//                
//                if(stepPath_pair1.second.collideWith(stepPath_pair2.second)){
//
//                    energy += token.getAgent_energy_system().getMovingRegime();
//
//                }
//
//            }
//            
//           
//            for (auto alloc_pair : solution.allocation_map){
//                
//                if(alloc_pair.first != stepPath_pair1.first){
//                    
//                    auto pathInitialPair1 = agentStepPaths.find(alloc_pair.first);                
//                    unsigned currentStep2 = pathInitialPair1->second.goalSite().GetStep();
//                    
//                    task_path.listPaths(
//                        token.getMap().getEndpointsPathAlgorithm(),
//                        alloc_pair.second, 
//                        [&solution, &stepPath_pair1, &token, &energy, &currentStep2](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){
//                                                        
//                            if(path2.size() > 0) {
//                                
//                                _stepPath stepPath2;
//                                _stepPath::buildFromPath(currentStep2, path2, stepPath2);
//                                currentStep2 = stepPath2.goalSite().GetStep();                            
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
//                                if(stepPath_pair1.second.collideWith(stepPath2)){
//
//                                    if(type2 == _task_path::PathType::delivery){ // delivery
//
//                                        energy += token.getAgent_energy_system().getCarringRegime();
//
//                                    } else { // pickup
//
//                                        energy += token.getAgent_energy_system().getMovingRegime();
//
//                                    } 
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
//        for (auto alloc_pair1 : solution.allocation_map) {
//
//            if(!alloc_pair1.second.empty()){
//                
//                auto pathInitialPair1 = agentStepPaths.find(alloc_pair1.first);                
//                unsigned currentStep1 = pathInitialPair1->second.goalSite().GetStep();   
//                                                                
//                task_path.listPaths(
//                    token.getMap().getEndpointsPathAlgorithm(),
//                    alloc_pair1.second, 
//                    [&solution, &alloc_pair1, &currentStep1, &token, &energy, &makespan, &agentStepPaths, this](const _task_path::PathType& type1, const _path& path1, const _task* curr, const _task* next){
//                                                
//                        if(path1.size() > 0) {
//                        
//                            _stepPath stepPath1;
//                            _stepPath::buildFromPath(currentStep1, path1, stepPath1);
//
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
//                            for (auto alloc_pair2 : solution.allocation_map){
//
//                                if(alloc_pair1.first != alloc_pair2.first){
//
//                                    if(!alloc_pair2.second.empty()){
//
//                                        auto pathInitialPair2 = agentStepPaths.find(alloc_pair2.first);                
//                                        unsigned currentStep2 = pathInitialPair2->second.goalSite().GetStep(); 
//
//                                        task_path.listPaths(
//                                            token.getMap().getEndpointsPathAlgorithm(),
//                                            alloc_pair2.second, 
//                                            [&solution, &stepPath1, &currentStep2, &token, &energy, &type1](const _task_path::PathType& type2, const _path& path2, const _task* curr, const _task* next){
//
//                                                if(path2.size() > 0) {
//                                                    
//                                                    _stepPath stepPath2;
//                                                    _stepPath::buildFromPath(currentStep2, path2, stepPath2);
//
//
//                                                    currentStep2 = stepPath2.goalSite().GetStep();
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
//                                                    if(stepPath1.collideWith(stepPath2)){
//
//                                                        if(type1 == _task_path::PathType::delivery){ // delivery
//
//                                                            energy += token.getAgent_energy_system().getCarringRegime();
//
//                                                        } else { // pickup
//
//                                                            energy += token.getAgent_energy_system().getMovingRegime();
//
//                                                        } 
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
//        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan));
//        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy));
//                
//    }
//    
//    return solution.evals;
//    
//}

//const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of_path_collision::evals(const _ga_token& token, _ga_solution& solution) const{
//    
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
//        unsigned makespan = 0;
//        double energy = .0;
//        std::map<const _ga_agent*, std::vector<_stepPath>> agentPaths;
////        std::map<const _ga_agent*,std::vector<const _task*>> allocation_copy;
//
//        for (auto alloc_pair : solution.allocation_map) {
//
////            allocation_copy.insert(std::pair<const _ga_agent*,std::vector<const _task*>>(alloc_pair.first, alloc_pair.second));
//
//            auto it = agentPaths.insert(std::pair<const _ga_agent*, std::vector<_stepPath>>(alloc_pair.first, std::vector<_stepPath>()));
//                        
//            _stepSite currentSite = it.first->first->currentSite();
//            
//            
//            
//            bool flag = true;
//            for (auto task : alloc_pair.second) {
//                
//                _stepPath stepPath;
//
//                if(true){
//                    
//                    _path path;
//                    
//                    flag = astar.solve(token.getMap(), currentSite, task->getPickup(), path);
//                    
//                    if(flag){
//                                              
//                        _stepPath::buildFromPath(currentSite.GetStep(),path, stepPath);
//                        
//                    } else {
//                        
//                        try {
//                            std::ostringstream stream;
//                            stream << "path not found from " << currentSite << " to " <<  task->getPickup();
//                            MAPD_EXCEPTION(stream.str());
//                        } catch (std::exception& e) {
//                            std::cout << e.what() << std::endl;
//                            std::abort();
//                        }
//                        
//                    }     
//                    
//                    flag = false;
//
//                } else {
//                    
//                    _stepPath::buildFromPath(
//                        currentSite.GetStep(), 
//                        token.getMap().getEndpointsPathAlgorithm().solve_path(currentSite, task->getPickup()), 
//                        stepPath);
//                                   
//
//                }
//                
//                it.first->second.push_back(stepPath);
//
//                currentSite = stepPath.goalSite();  
//                
//                
//                _stepPath::buildFromPath(
//                    currentSite.GetStep(), 
//                    token.getMap().getEndpointsPathAlgorithm().solve_path(task->getPickup(), task->getDelivery()), 
//                    stepPath);
//                        
//                it.first->second.push_back(stepPath);
//
//                currentSite = stepPath.goalSite();                      
//
//            }
//
//        }
//
//        
//        
//        for (auto agentSites_pair1 : agentPaths) {
//            
//            if( !agentSites_pair1.second.empty() 
//                && agentSites_pair1.second.back().goalSite().GetStep() > makespan){
//
//                makespan = agentSites_pair1.second.back().goalSite().GetStep();
//
//            }
//            
//            unsigned index = 0;
//            
//            for(const auto& path1: agentSites_pair1.second){
//                
//                for (auto agentSites_pair2 : agentPaths) {
//                    
//                    if(agentSites_pair1.first != agentSites_pair2.first){
//                        
//                        for(const auto& path2: agentSites_pair2.second){
//                            
//                            if(path1.goalSite().GetStep() < path2.currentSite().GetStep()){
//                                
//                                break;
//                                
//                            } else if(path1.currentSite().GetStep() > path2.goalSite().GetStep()){
//                                
//                                continue;
//                                
//                            }
//                            
//                            double energy_cost = .0;
//                                                        
//                            if(path1.collideWith(path2)){
//            
//                                if(index % 2 == 0){ // delivery
//
//                                    energy_cost = token.getAgent_energy_system().getCarringRegime();
//
//                                } else { // pickup
//
//                                    energy_cost = token.getAgent_energy_system().getMovingRegime();
//
//                                } 
//                            
//                            }
//                            
//                            energy += energy_cost;
//                        
//                        }
//                    
//                    }
//
//                }
//                
//                index++;
//                
//            }           
//            
//        }
//
//        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan));
//        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy));
//        
//        
//    }
//    
//    return solution.evals;
//    
//}