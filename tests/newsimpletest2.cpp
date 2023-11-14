/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/testFiles/simpletest.cpp to edit this template
 */

/* 
 * File:   newsimpletest2.cpp
 * Author: ronaldo
 *
 * Created on 13 de novembro de 2023, 14:14
 */

#include <stdlib.h>
#include <iostream>

#include "_stepMap.h"

/*
 * Simple C++ Test Suite
 */

void test1() {
    
    _stepMap sm(5,5,5);
    
    for (int r = 0; r < 5; r++)        
        for (int c = 0; c < 5; c++)
            sm.resetTypesInStepColunm(r, c, _stepMap::NodeType::freeNode);
        
    sm.resetTypesInStepColunm(0,0,0);
    
    
    sm.stepView(0,4);
    sm.free_agent_view();
    sm.free_step_view();
    
    _stepPath sp1, sp2;
    
    sp1.progress(_stepSite(0,0,0));
    sp1.progress(_stepSite(1,0,1));
    sp1.progress(_stepSite(2,1,1));
    
    sm.setMoving(sp1, 0);
    
    sm.stepView(0,4);
    sm.free_agent_view();
    sm.free_step_view();
    
    
    sp2.progress(_stepSite(2,1,1));
    sp2.progress(_stepSite(3,1,2));
    sp2.progress(_stepSite(4,1,3));
    
    sm.setMoving(sp2, 0);
    
    sm.stepView(0,4);
    sm.free_agent_view();
    sm.free_step_view();
    
    
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

