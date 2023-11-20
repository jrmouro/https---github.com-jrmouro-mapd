/* 
 * File:   newsimpletest2.cpp
 * Author: ronaldo
 *
 * Created on 13 de novembro de 2023, 14:14
 */

#include <stdlib.h>
#include <iostream>

#include "_stepMap.h"
#include "_stepAstarAlgorithm.h"

/*
 * Simple C++ Test Suite
 */

void test1() {
    
    _stepAstarAlgorithm astar;
    
    _stepMap sm(50,5,5);
    
    for (int r = 0; r < 5; r++)        
        for (int c = 0; c < 5; c++)
            sm.resetTypesInStepColunm(r, c, _stepMap::NodeType::freeNode);
        
    sm.resetTypesInStepColunm(2,0,0);
    sm.resetTypesInStepColunm(1,2,1);
    
    
    sm.stepView(0,1);
    sm.free_agent_view();
    sm.max_step_view();
    
    _stepPath p0(_stepSite(0, 2, 0)), p1(_stepSite(0, 1, 2));
    
    bool flag = astar.solve(sm, p0, _site(2, 4), 0);
    
    if(flag){
        
        std::cout << p0 << std::endl;
        
        sm.setMoving(p0, 0);
        
        sm.stepView(1,5);
        sm.free_agent_view();
        sm.max_step_view();
        
    } else {
        
        std::cout << "path not found" << std::endl;
        
    }
    
    flag = astar.solve(sm, p1, _site(2, 2), 1);
    
    if(flag){
        
        std::cout << p1 << std::endl;
        
        sm.setMoving(p1, 1);
        
        sm.stepView(1,5);
        sm.free_agent_view();
        sm.max_step_view();
        
    } else {
        
        std::cout << "path not found" << std::endl;
        
    }
    
    _stepSite current = p0.goalSite();
    p0.clear();
    p0.add(current);
    
    flag = astar.solve(sm, p0, _site(2, 4), 0);
    
    if(flag){
        
        std::cout << p0 << std::endl;
        
        sm.setMoving(p0, 0);
        
        sm.stepView(5,9);
        sm.free_agent_view();
        sm.max_step_view();
                
        current = p0.currentSite();
                
        sm.deleteMoving(p0, 0);
        
        p0.clear();
        p0.add(current);
    
        flag = astar.solve(sm, p0, _site(2, 0), 0);
        
        if(flag){
            
            std::cout << p0 << std::endl;
            
            sm.setMoving(p0, 0);
            
            sm.stepView(5,11);
            sm.free_agent_view();
            sm.max_step_view();
            
        }else{
            
            std::cout << "path not found" << std::endl;
            
        }
        
        
    } else {
        
        std::cout << "path not found" << std::endl;
        
    }
    
    
    
    
    
    
    
    
    
    std::cout << "newsimpletest2 test 1" << std::endl;
}

void test2() {
    std::cout << "newsimpletest2 test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest2) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    
    
    
    
    
    
    
    
    
    std::cout << "%SUITE_STARTING% newsimpletest2" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest2)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest2)" << std::endl;

    

    return (EXIT_SUCCESS);
}

