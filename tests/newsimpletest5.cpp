
/* 
 * File:   newsimpletest5.cpp
 * Author: ronaldo
 *
 * Created on 28 de novembro de 2023, 20:19
 */

#include <stdlib.h>
#include <iostream>

#include "InstanceMAPD.h"
#include "_task_path.h"

/*
 * Simple C++ Test Suite
 */

void test_task_path() {
    
    auto imap = InstanceMap::load("./Instances/test/test_2.map");
    _map map(imap->getMap());
                
    auto itasks = InstanceTask::load("./Instances/test/test_1.task", [&map](unsigned id){
        return *map.getNoBotEndPointById(id);
    });

    std::vector<const _task*> tasks;
    
    itasks->getTaskMap().listTasks([&map, &tasks](unsigned step, const _task& task){  
        tasks.push_back(&task);
        map.setTypeOfSite(task.getPickup().GetRow(), task.getPickup().GetColunm(), _map::TypeOfSite::task);
        map.setTypeOfSite(task.getDelivery().GetRow(), task.getDelivery().GetColunm(), _map::TypeOfSite::task);           
        return false;
    });
    
    
    std::vector<const _task*> tasks1, tasks2;
    
    tasks1.push_back(tasks[9]);
    tasks1.push_back(tasks[15]);
    
    tasks2.push_back(tasks[7]);
    tasks2.push_back(tasks[11]);
    
    
    _stepPath path;
    path.progress(_stepSite(3, 2, 8));
    path.progress(_stepSite(4, 2, 7));
    path.progress(_stepSite(5, 2, 6));
    path.progress(_stepSite(6, 3, 6));
    path.progress(_stepSite(7, 4, 6));
        
    
    unsigned portion = 2;
    _task_path task_path(portion);
    
    task_path.listPaths(
            map.getEndpointsPathAlgorithm(), 
            tasks1, 
            [](const _task_path::PathType& type, const _path& path, const _task*, const _task*){
                
                std::cout << "PathType: " << type << std::endl << path << std::endl;
                
                return false;
                
            });
            
            unsigned ps, ds;
            
    task_path.listSites(
            map.getEndpointsPathAlgorithm(), 
            tasks1,
            0,
            ps, ds,
            [](const _task_path::PathType& type, const _site& site, const _task*, const _task*){
                
                std::cout << "PathType: " << type << " : " << site << std::endl;
                
                return false;
                
            });
    
    std::cout << "ps: " << ps << std::endl;
    std::cout << "ds: " << ds << std::endl;
    
    task_path.listPaths(
            map.getEndpointsPathAlgorithm(), 
            tasks2, 
            [](const _task_path::PathType& type, const _path& path, const _task*, const _task*){
                
                std::cout << "PathType: " << type << std::endl << path << std::endl;
                
                return false;
                
            });
        
            
            
    task_path.listSites(
            map.getEndpointsPathAlgorithm(), 
            tasks2,
            0,
            ps, ds,
            [](const _task_path::PathType& type, const _site& site, const _task*, const _task*){
                
                std::cout << "PathType: " << type << " : " << site << std::endl;
                
                return false;
                
            });
            
    std::cout << "ps: " << ps << std::endl;
    std::cout << "ds: " << ds << std::endl;
            
    task_path.listSiteMatches(
            map.getEndpointsPathAlgorithm(), 
            tasks1,
            tasks2,
            1,
            0,
            ps,
            ds,
            [](const _task_path::PathType& t1, const _task_path::PathType& t2, const _site& site1, const _site& site2){
                
                std::cout << "match: "  << std::endl;
                std::cout << "PathType1: " << t1 << " : " << site1 << std::endl;
                std::cout << "PathType2: " << t2 << " : " << site2 << std::endl;
                
                return false;
            },1);
            
    std::cout << "ps: " << ps << std::endl;
    std::cout << "ds: " << ds << std::endl;
            
    unsigned p1, p2, d1, d2;
    task_path.countMatches(
            map.getEndpointsPathAlgorithm(), 
            tasks1,
            tasks2,
            1,
            0,
            p1, p2, d1, d2, ps, ds, 1);
    
    std::cout << "ps: " << ps << std::endl;
    std::cout << "ds: " << ds << std::endl;
        
    std::cout << "P1: " << p1 << std::endl;
    std::cout << "P2: " << p2 << std::endl;
    std::cout << "D1: " << d1 << std::endl;
    std::cout << "D2: " << d2 << std::endl;
    
    std::cout << std::endl;
    
    task_path.countMatchesWithStepPath(
            map.getEndpointsPathAlgorithm(), 
            tasks2,
            path,
            p1, d1, ps, ds, 1);
    
    std::cout << "ps: " << ps << std::endl;
    std::cout << "ds: " << ds << std::endl;
    
    std::cout << "P1: " << p1 << std::endl;
    std::cout << "D1: " << d1 << std::endl;
    
    
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=test_task_path (newsimpletest5) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest5" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test_task_path (newsimpletest5)" << std::endl;
    test_task_path();
    std::cout << "%TEST_FINISHED% time=0 test_task_path (newsimpletest5)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

