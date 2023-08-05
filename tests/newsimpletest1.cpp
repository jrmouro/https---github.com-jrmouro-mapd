/* 
 * File:   newsimpletest1.cpp
 * Author: ronaldo
 *
 * Created on 1 de agosto de 2023, 04:55
 */

#include <stdlib.h>
#include <iostream>
#include "InstanceMapExperiment.h"

/*
 * Simple C++ Test Suite
 */

void testRun() {
    InstanceMapExperiment instanceMapExperiment("./Instances/small/kiva-50-500-5.map");
    
    instanceMapExperiment.run();
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=testRun (newsimpletest1) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest1" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testRun (newsimpletest1)" << std::endl;
    testRun();
    std::cout << "%TEST_FINISHED% time=0 testRun (newsimpletest1)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

