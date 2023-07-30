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
        "./Instances/small/kiva-0.2.task",
        "./Instances/small/kiva-50-500-5.map", 
        true);
    
    experiment.run();
    

    return 0;
    
}

