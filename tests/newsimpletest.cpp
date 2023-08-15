/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/testFiles/simpletest.cpp to edit this template
 */

/* 
 * File:   newsimpletest.cpp
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 11:22
 */

#include <stdlib.h>
#include <iostream>
#include "AstarExperiment.h"

/*
 * Simple C++ Test Suite
 */

void testAstarExperiment() {
    std::string taskFilename = "./Instances/test/test.task";
    std::string mapFilename = "./Instances/test/test.map";
    bool render = true;
    unsigned taskId = 2;
    AstarExperiment astarExperiment(taskFilename, mapFilename, render, taskId);
    astarExperiment.run();
    
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=testAstarExperiment (newsimpletest) message=error message sample" << std::endl;
    }
}


int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testAstarExperiment (newsimpletest)" << std::endl;
    testAstarExperiment();
    std::cout << "%TEST_FINISHED% time=0 testAstarExperiment (newsimpletest)" << std::endl;


    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

