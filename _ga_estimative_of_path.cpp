/* 
 * File:   _ga_estimative_of_path.cpp
 * Author: ronaldo
 * 
 * Created on 9 de dezembro de 2023, 21:58
 */

#include "_ga_estimative_of_path.h"

#include "_endpointsPathAlgorithm.h"
#include "_task_path.h"

void _ga_estimative_of_path::pathSwap(
    const _ga_token& token, 
    const _ga_solution& solution, 
    unsigned& makespan,
    unsigned& energy,
    std::map<const _ga_agent*, _path>& agentPaths,
    std::vector<std::vector<const _site*>>& paths,
    std::vector<std::vector<_task_path::PathType>>& types) const{
    
    _astarAlgorithm astar;
    _task_path task_path;
    
    paths.resize(solution.allocation_map.size(), std::vector<const _site*>());
    types.resize(solution.allocation_map.size(), std::vector<_task_path::PathType>());
    
    int index = 0;
    for (auto const& alloc_pair1 : solution.allocation_map) {
        
        auto it = agentPaths.insert(
                std::pair<const _ga_agent*, _path>(
                    alloc_pair1.first, 
                    _path()));
        
        const _task* task = alloc_pair1.first->getCurrentTask();
        
        unsigned taskIndex = 0;
        
        if(task != nullptr){
                                    
            if(alloc_pair1.second.empty()){
            
                bool flag2 = false;
                
                alloc_pair1.first->getPath().forward([&flag2, &alloc_pair1, &token, &paths, &types, &energy, index, task](const _stepSite& site){

                    if(alloc_pair1.first->isGoingToDelivering() || flag2){                        

                        energy += token.getAgent_energy_system().getCarringRegime();
                        types[index].push_back(_task_path::PathType::delivery);

                    } else {

                        energy += token.getAgent_energy_system().getMovingRegime();
                        types[index].push_back(_task_path::PathType::pickup);
                    }

                    paths[index].push_back(&site);

                    if(!flag2 && task->getPickup().match(site)){
                        flag2 = true;
                    }

                    return false;

                }); 
            
            } else {
                
                if(task->id() == alloc_pair1.second[0]->id()){
                    
                    bool flag2 = false;
                
                    alloc_pair1.first->getPath().forward([&flag2, &alloc_pair1, &token, &paths, &types, &energy, index, task](const _stepSite& site){

                        if(alloc_pair1.first->isGoingToDelivering() || flag2){                        

                            energy += token.getAgent_energy_system().getCarringRegime();
                            types[index].push_back(_task_path::PathType::delivery);

                        } else {

                            energy += token.getAgent_energy_system().getMovingRegime();
                            types[index].push_back(_task_path::PathType::pickup);
                        }

                        paths[index].push_back(&site);

                        if(!flag2 && task->getPickup().match(site)){
                            flag2 = true;
                        }

                        return false;

                    });   
                    
                    taskIndex = 1;
                                        
                } else {
                                    
                    bool flag2 = astar.solve(token.getMap(), alloc_pair1.first->currentSite(), alloc_pair1.second[0]->getPickup(), it.first->second);

                    if(flag2){

                        it.first->second.rlist([&token, &paths, &types, &energy, index](const _site& site){

                            paths[index].push_back(&site);
                            types[index].push_back(_task_path::PathType::pickup);

                            energy += token.getAgent_energy_system().getMovingRegime();

                            return false;

                        });                                      

                    } else {

                        try {
                            std::ostringstream stream;
                            stream << "path not found from " << alloc_pair1.first->currentSite() << " to " <<  alloc_pair1.second[0]->getPickup();
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }

                    }
                    
                }
                
            }
            
        } else {
            
            if(alloc_pair1.second.empty()){
                
                alloc_pair1.first->getPath().forward([&alloc_pair1, &token, &paths, &types, &energy, index](const _stepSite& site){

                    energy += token.getAgent_energy_system().getMovingRegime();
                    types[index].push_back(_task_path::PathType::pickup);
                    paths[index].push_back(&site);

                    return false;

                });  
                
            } else {
                                  
                bool flag2 = astar.solve(token.getMap(), alloc_pair1.first->currentSite(), alloc_pair1.second[0]->getPickup(), it.first->second);
                
                if(flag2){
                    
                    it.first->second.rlist([&token, &paths, &types, &energy, index](const _site& site){

                        paths[index].push_back(&site);
                        types[index].push_back(_task_path::PathType::pickup);

                        energy += token.getAgent_energy_system().getMovingRegime();

                        return false;

                    });                                      

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "path not found from " << alloc_pair1.first->currentSite() << " to " <<  alloc_pair1.second[0]->getPickup();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }
                
            }
            
        }
        
        unsigned pickup_span, delivary_span;

        task_path.listSites(
            token.getMap().getEndpointsPathAlgorithm(),
            alloc_pair1.second, 
            taskIndex, 
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
    
    
    unsigned max_size = 0;
    
    for(auto const& path:paths){
        
        if(max_size < path.size()){
            
            max_size = path.size();
            
        }
        
    }
    
    for(int i = 0; i < paths.size(); i++){
        
        const _site* site = paths[i].back();
        
        for(int j = paths[i].size(); j < max_size; j++){
            
            paths[i].push_back(site);
            types[i].push_back(_task_path::PathType::pickup);
            
            energy += token.getAgent_energy_system().getUnloadedRegime();
            
        }
        
    }
       
}
        
void _ga_estimative_of_path::path(
    const _ga_token& token, 
    const _ga_solution& solution, 
    unsigned& makespan,
    unsigned& energy,
    std::map<const _ga_agent*, _path>& agentPaths,
    std::vector<std::vector<const _site*>>& paths,
    std::vector<std::vector<_task_path::PathType>>& types)const{
    
    _astarAlgorithm astar;
    _task_path task_path;
    
    paths.resize(solution.allocation_map.size(), std::vector<const _site*>());
    types.resize(solution.allocation_map.size(), std::vector<_task_path::PathType>());
    
    int index = 0;
    for (auto const& alloc_pair1 : solution.allocation_map) {
        
        auto it = agentPaths.insert(
                std::pair<const _ga_agent*, _path>(
                    alloc_pair1.first, 
                    _path()));
        
        const _task* task = alloc_pair1.first->getCurrentTask();
        
        unsigned taskIndex = 0;
        
        if(task != nullptr){
            
            bool flag2 = false;
                
            alloc_pair1.first->getPath().forward([&flag2, &alloc_pair1, &token, &paths, &types, &energy, index, task](const _stepSite& site){

                if(alloc_pair1.first->isGoingToDelivering() || flag2){                        

                    energy += token.getAgent_energy_system().getCarringRegime();
                    types[index].push_back(_task_path::PathType::delivery);

                } else {

                    energy += token.getAgent_energy_system().getMovingRegime();
                    types[index].push_back(_task_path::PathType::pickup);
                }

                paths[index].push_back(&site);

                if(!flag2 && task->getPickup().match(site)){
                    flag2 = true;
                }

                return false;

            }); 
            
            if(!alloc_pair1.second.empty() && task->id() == alloc_pair1.second[0]->id()){
            
                taskIndex = 1;
            
            }
            
        } else {
            
            if(alloc_pair1.second.empty()){
                
                alloc_pair1.first->getPath().forward([&alloc_pair1, &token, &paths, &types, &energy, index](const _stepSite& site){

                    energy += token.getAgent_energy_system().getMovingRegime();
                    types[index].push_back(_task_path::PathType::pickup);
                    paths[index].push_back(&site);

                    return false;

                });  
                
            } else {
                
                bool flag2 = astar.solve(token.getMap(), alloc_pair1.first->currentSite(), alloc_pair1.second[0]->getPickup(), it.first->second);
                
                if(flag2){
                    
                    it.first->second.rlist([&token, &paths, &types, &energy, index](const _site& site){

                        paths[index].push_back(&site);
                        types[index].push_back(_task_path::PathType::pickup);

                        energy += token.getAgent_energy_system().getMovingRegime();

                        return false;

                    });                                      

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "path not found from " << alloc_pair1.first->currentSite() << " to " <<  alloc_pair1.second[0]->getPickup();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }
                
            }
            
        }
        
        unsigned pickup_span, delivary_span;

        task_path.listSites(
            token.getMap().getEndpointsPathAlgorithm(),
            alloc_pair1.second, 
            taskIndex, 
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
    
    unsigned max_size = 0;
    
    for(auto const& path:paths){
        
        if(max_size < path.size()){
            
            max_size = path.size();
            
        }
        
    }
    
    for(int i = 0; i < paths.size(); i++){
        
        const _site* site = paths[i].back();
        
        for(int j = paths[i].size(); j < max_size; j++){
            
            paths[i].push_back(site);
            types[i].push_back(_task_path::PathType::pickup);
            
            energy += token.getAgent_energy_system().getUnloadedRegime();
            
        }
        
    }
    
}

void _ga_estimative_of_path::pathSwapTaskEndpoint(
    const _ga_token& token, 
    const _ga_solution& solution,
    unsigned& makespan,
    unsigned& energy,
    std::map<const _ga_agent*, _path>& agentPaths,
    std::vector<std::vector<const _site*>>& paths,
    std::vector<std::vector<_task_path::PathType>>& types,
    std::vector<std::vector<unsigned>>& pickupv,
    std::vector<std::vector<unsigned>>& deliveryv)const{

    _astarAlgorithm astar;
    _task_path task_path;
    
    paths.resize(solution.allocation_map.size(), std::vector<const _site*>());
    types.resize(solution.allocation_map.size(), std::vector<_task_path::PathType>());
    pickupv.resize(solution.allocation_map.size(), std::vector<unsigned>());
    deliveryv.resize(solution.allocation_map.size(), std::vector<unsigned>());
    
    int index = 0;
    for (auto const& alloc_pair1 : solution.allocation_map) {
        
        auto it = agentPaths.insert(
                std::pair<const _ga_agent*, _path>(
                    alloc_pair1.first, 
                    _path()));
        
        const _task* task = alloc_pair1.first->getCurrentTask();
        
        unsigned taskIndex = 0;
        
        if(task != nullptr){
                                    
            if(alloc_pair1.second.empty()){
            
                bool flag2 = false;
                
                alloc_pair1.first->getPath().forward([&flag2, &alloc_pair1, &token, &paths, &types, &pickupv, &deliveryv, &energy, index, task, this](const _stepSite& site){

                    if(alloc_pair1.first->isGoingToDelivering() || flag2){                        

                        energy += token.getAgent_energy_system().getCarringRegime();
                        types[index].push_back(_task_path::PathType::delivery);

                    } else {

                        energy += token.getAgent_energy_system().getMovingRegime();
                        types[index].push_back(_task_path::PathType::pickup);
                    }

                    paths[index].push_back(&site);

                    if(!flag2 && task->getPickup().match(site)){
                        flag2 = true;
                    }
                    
                    if(site.match(task->getPickup())){
                        pickupv[index].push_back(pickup_energy_penalty);
                        deliveryv[index].push_back(0);
                    } else if(site.match(task->getDelivery())){
                        pickupv[index].push_back(0);
                        deliveryv[index].push_back(delivery_energy_penalty);
                    } else {
                        pickupv[index].push_back(0);
                        deliveryv[index].push_back(0);
                    }

                    return false;

                }); 
            
            } else {
                
                if(task->id() == alloc_pair1.second[0]->id()){
                    
                    bool flag2 = false;
                
                    alloc_pair1.first->getPath().forward([&flag2, &alloc_pair1, &token, &paths, &types, &pickupv, &deliveryv, &energy, index, task, this](const _stepSite& site){

                        if(alloc_pair1.first->isGoingToDelivering() || flag2){                        

                            energy += token.getAgent_energy_system().getCarringRegime();
                            types[index].push_back(_task_path::PathType::delivery);

                        } else {

                            energy += token.getAgent_energy_system().getMovingRegime();
                            types[index].push_back(_task_path::PathType::pickup);
                        }

                        paths[index].push_back(&site);

                        if(!flag2 && task->getPickup().match(site)){
                            flag2 = true;
                        }
                        
                        if(site.match(task->getPickup())){
                            pickupv[index].push_back(pickup_energy_penalty);
                            deliveryv[index].push_back(0);
                        } else if(site.match(task->getDelivery())){
                            pickupv[index].push_back(0);
                            deliveryv[index].push_back(delivery_energy_penalty);
                        } else {
                            pickupv[index].push_back(0);
                            deliveryv[index].push_back(0);
                        }

                        return false;

                    });   
                    
                    taskIndex = 1;
                                        
                } else {
                                    
                    bool flag2 = astar.solve(token.getMap(), alloc_pair1.first->currentSite(), alloc_pair1.second[0]->getPickup(), it.first->second);

                    if(flag2){

                        it.first->second.rlist([&token, &paths, &types, &energy, index, &pickupv, &deliveryv](const _site& site){

                            paths[index].push_back(&site);
                            types[index].push_back(_task_path::PathType::pickup);

                            energy += token.getAgent_energy_system().getMovingRegime();
                                                        
                            pickupv[index].push_back(0);
                            deliveryv[index].push_back(0);                            

                            return false;

                        });                                      

                    } else {

                        try {
                            std::ostringstream stream;
                            stream << "path not found from " << alloc_pair1.first->currentSite() << " to " <<  alloc_pair1.second[0]->getPickup();
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }

                    }
                    
                }
                
            }
            
        } else {
            
            if(alloc_pair1.second.empty()){
                
                alloc_pair1.first->getPath().forward([&alloc_pair1, &token, &paths, &types, &energy, index, &pickupv, &deliveryv](const _stepSite& site){

                    energy += token.getAgent_energy_system().getMovingRegime();
                    types[index].push_back(_task_path::PathType::pickup);
                    paths[index].push_back(&site);
                    pickupv[index].push_back(0);
                    deliveryv[index].push_back(0);

                    return false;

                });  
                
            } else {
                                  
                bool flag2 = astar.solve(token.getMap(), alloc_pair1.first->currentSite(), alloc_pair1.second[0]->getPickup(), it.first->second);
                
                if(flag2){
                    
                    it.first->second.rlist([&token, &paths, &types, &energy, index, &pickupv, &deliveryv](const _site& site){

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
                        stream << "path not found from " << alloc_pair1.first->currentSite() << " to " <<  alloc_pair1.second[0]->getPickup();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }
                
            }
            
        }
        
        unsigned pickup_span, delivary_span;

        task_path.listSites(
            token.getMap().getEndpointsPathAlgorithm(),
            alloc_pair1.second, 
            taskIndex, 
            1,
            pickup_span,
            delivary_span,
            [&token, &paths, &types, &energy, index, &pickupv, &deliveryv, this](const _task_path::PathType& type, const _site& site, const _task* t1, const _task* t2){

                paths[index].push_back(&site);
                types[index].push_back(type);
                
                if(site.match(t1->getPickup())){
                    pickupv[index].push_back(pickup_energy_penalty);
                    deliveryv[index].push_back(0);
                } else if(site.match(t1->getDelivery())){
                    pickupv[index].push_back(0);
                    deliveryv[index].push_back(delivery_energy_penalty);
                } else {
                    pickupv[index].push_back(0);
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
    
    
    unsigned max_size = 0;
    
    for(auto const& path:paths){
        
        if(max_size < path.size()){
            
            max_size = path.size();
            
        }
        
    }
    
    for(int i = 0; i < paths.size(); i++){
        
        const _site* site = paths[i].back();
        
        for(int j = paths[i].size(); j < max_size; j++){
            
            paths[i].push_back(site);
            types[i].push_back(_task_path::PathType::pickup);
            pickupv[i].push_back(0);
            deliveryv[i].push_back(0);
            
            energy += token.getAgent_energy_system().getUnloadedRegime();
            
        }
        
    }
    
}
        
void _ga_estimative_of_path::pathTaskEndpoint(
    const _ga_token& token, 
    const _ga_solution& solution,
    unsigned& makespan,
    unsigned& energy,
    std::map<const _ga_agent*, _path>& agentPaths,
    std::vector<std::vector<const _site*>>& paths,
    std::vector<std::vector<_task_path::PathType>>& types,
    std::vector<std::vector<unsigned>>& pickupv,
    std::vector<std::vector<unsigned>>& deliveryv)const{

    _astarAlgorithm astar;
    _task_path task_path;
    
    paths.resize(solution.allocation_map.size(), std::vector<const _site*>());
    types.resize(solution.allocation_map.size(), std::vector<_task_path::PathType>());
    pickupv.resize(solution.allocation_map.size(), std::vector<unsigned>());
    deliveryv.resize(solution.allocation_map.size(), std::vector<unsigned>());
    
    int index = 0;
    for (auto const& alloc_pair1 : solution.allocation_map) {
        
        auto it = agentPaths.insert(
                std::pair<const _ga_agent*, _path>(
                    alloc_pair1.first, 
                    _path()));
        
        const _task* task = alloc_pair1.first->getCurrentTask();
        
        unsigned taskIndex = 0;
        
        if(task != nullptr){
            
            bool flag2 = false;
                
            alloc_pair1.first->getPath().forward([&flag2, &alloc_pair1, &token, &paths, &types, &pickupv, &deliveryv, &energy, index, task, this](const _stepSite& site){

                if(alloc_pair1.first->isGoingToDelivering() || flag2){                        

                    energy += token.getAgent_energy_system().getCarringRegime();
                    types[index].push_back(_task_path::PathType::delivery);

                } else {

                    energy += token.getAgent_energy_system().getMovingRegime();
                    types[index].push_back(_task_path::PathType::pickup);
                }

                paths[index].push_back(&site);
                
                if(site.match(task->getPickup())){
                    pickupv[index].push_back(pickup_energy_penalty);
                    deliveryv[index].push_back(0);
                } else if(site.match(task->getDelivery())){
                    pickupv[index].push_back(0);
                    deliveryv[index].push_back(delivery_energy_penalty);
                } else {
                    pickupv[index].push_back(0);
                    deliveryv[index].push_back(0);
                }

                if(!flag2 && task->getPickup().match(site)){
                    flag2 = true;
                }

                return false;

            }); 
            
            if(!alloc_pair1.second.empty() && task->id() == alloc_pair1.second[0]->id()){
            
                taskIndex = 1;
            
            }
            
        } else {
            
            if(alloc_pair1.second.empty()){
                
                alloc_pair1.first->getPath().forward([&alloc_pair1, &token, &paths, &types, &pickupv, &deliveryv, &energy, index](const _stepSite& site){

                    energy += token.getAgent_energy_system().getMovingRegime();
                    types[index].push_back(_task_path::PathType::pickup);
                    paths[index].push_back(&site);
                    pickupv[index].push_back(0);
                    deliveryv[index].push_back(0);

                    return false;

                });  
                
            } else {
                
                bool flag2 = astar.solve(token.getMap(), alloc_pair1.first->currentSite(), alloc_pair1.second[0]->getPickup(), it.first->second);
                
                if(flag2){
                    
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
                        stream << "path not found from " << alloc_pair1.first->currentSite() << " to " <<  alloc_pair1.second[0]->getPickup();
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }
                
            }
            
        }
        
        unsigned pickup_span, delivary_span;

        task_path.listSites(
            token.getMap().getEndpointsPathAlgorithm(),
            alloc_pair1.second, 
            taskIndex, 
            1,
            pickup_span,
            delivary_span,
            [&token, &paths, &types, &pickupv, &deliveryv, &energy, index, this](const _task_path::PathType& type, const _site& site, const _task* t1, const _task* t2){

                paths[index].push_back(&site);
                types[index].push_back(type);
                
                if(site.match(t1->getPickup())){
                    pickupv[index].push_back(pickup_energy_penalty);
                    deliveryv[index].push_back(0);
                } else if(site.match(t1->getDelivery())){
                    pickupv[index].push_back(0);
                    deliveryv[index].push_back(delivery_energy_penalty);
                } else {
                    pickupv[index].push_back(0);
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
    
    unsigned max_size = 0;
    
    for(auto const& path:paths){
        
        if(max_size < path.size()){
            
            max_size = path.size();
            
        }
        
    }
    
    for(int i = 0; i < paths.size(); i++){
        
        const _site* site = paths[i].back();
        
        for(int j = paths[i].size(); j < max_size; j++){
            
            paths[i].push_back(site);
            types[i].push_back(_task_path::PathType::pickup);
            pickupv[i].push_back(0);
            deliveryv[i].push_back(0);
            
            energy += token.getAgent_energy_system().getUnloadedRegime();
            
        }
        
    }
    
}

