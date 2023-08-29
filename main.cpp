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
    
//    auto experiment = SystemExperiment(    
//        "./Instances/test/test_1.task",
//        "./Instances/test/test_1.map");
    
    auto experiment = SystemExperiment(    
        "./Instances/test/test_1.task", // instance tasks
        "./Instances/test/test_1.map", // instance map
        _system::TokenType::tp, // type of token
        1.0f, // task threshold
        1.0f, // carry threshold
        4000, // current agent level energy
        5000, // maximum agent level energy
        3000, // charging agent level energy
        500); // critical agent level energy
    
//    auto experiment = SystemExperiment(    
//        "./Instances/small/kiva-1.task", // instance tasks
//        "./Instances/small/kiva-10-500-5.map", // instance map
//        _system::TokenType::tp, // type of token
//        1.0f, // task threshold
//        1.0f, // carry threshold
//        4000, // current agent level energy
//        5000, // maximum agent level energy
//        3000, // charging agent level energy
//        500); // critical agent level energy
    
    experiment.run();
    

    return 0;
    
}

