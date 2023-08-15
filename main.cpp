/* 
 * File:   main.cpp
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:31
 */


#include "SystemExperiment.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    auto experiment = SystemExperiment(    
        "./Instances/test/test_1.task",
        "./Instances/test/test_1.map");
    
    experiment.run();
    

    return 0;
    
}

