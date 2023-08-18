/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/testFiles/simpletest.cpp to edit this template
 */

/* 
 * File:   newsimpletest1.cpp
 * Author: ronaldo
 *
 * Created on 17 de agosto de 2023, 12:28
 */

#include <stdlib.h>
#include <iostream>
#include "_agent.h"

/*
 * Simple C++ Test Suite
 */

void test_agent() {
    int _id;
    const _stepSite currentSite;
    _agent __agent(_id, currentSite);
    
    std::cout << __agent << std::endl;
    
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=test_agent (newsimpletest1) message=error message sample" << std::endl;
    }
}

void test_agent2() {
    int _id;
    const _stepSite currentSite;
    _agent agent(_id, currentSite);
    
    
    
    _agent __agent(agent);
    
    std::cout << __agent << std::endl;
    
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=test_agent2 (newsimpletest1) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest1" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test_agent (newsimpletest1)" << std::endl;
    test_agent();
    std::cout << "%TEST_FINISHED% time=0 test_agent (newsimpletest1)" << std::endl;

    std::cout << "%TEST_STARTED% test_agent2 (newsimpletest1)" << std::endl;
    test_agent2();
    std::cout << "%TEST_FINISHED% time=0 test_agent2 (newsimpletest1)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

