/* 
 * File:   main.cpp
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:31
 */


#include "AstarExperiment.h"
#include "AstarAlgorithm.h"
#include "Render.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    auto experiment = AstarExperiment(    
        "./Instances/test/test.task",
        "./Instances/test/test.map", 
        true,
        1);
    
    experiment.run();
    

    return 0;
    
}

