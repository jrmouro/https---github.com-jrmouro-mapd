/* 
 * File:   main.cpp
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:31
 */


#include "MultiSystemExperiment.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    std::string resultFile = "./../../charts/result.csv";
    std::vector<std::string> mapFilenames, taskFilenames;
    std::vector<_system::TokenType> systemTokenTypes;
    
    mapFilenames.push_back("./Instances/test/test_0.map");
    mapFilenames.push_back("./Instances/test/test_1.map");
    mapFilenames.push_back("./Instances/test/test_2.map");
    
    taskFilenames.push_back("./Instances/test/test_0.task");
    taskFilenames.push_back("./Instances/test/test_1.task");
    taskFilenames.push_back("./Instances/test/test_2.task");
    
    systemTokenTypes.push_back(_system::TokenType::tp);
    systemTokenTypes.push_back(_system::TokenType::taskThreshold_tp);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        taskFilenames,                      // instance tasks
        mapFilenames,                       // instance map
        systemTokenTypes,                   // type of token
        1.0f,                               // task threshold
        1.0f,                               // carry threshold
        350,                                // current agent level energy
        500,                                // maximum agent level energy
        400,                                // charged agent level energy
        70,                                // critical agent level energy
        128,                                 // cell_size
        0);                               // timestep
           

    experiment.run();    

    return 0;
    
}

