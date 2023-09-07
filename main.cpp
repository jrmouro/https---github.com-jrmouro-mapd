/* 
 * File:   main.cpp
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:31
 */


#include "MultiSystemExperiment.h"

using namespace std;

void test(){
    
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
    systemTokenTypes.push_back(_system::TokenType::carryThreshold_tp);
    systemTokenTypes.push_back(_system::TokenType::taskCarryThreshold_tp);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        taskFilenames,                      // instance tasks
        mapFilenames,                       // instance map
        systemTokenTypes,                   // type of token
        1.0f,                               // task threshold
        1.0f,                               // carry threshold
        600,                                // current agent level energy
        1000,                                // maximum agent level energy
        600,                                // charged agent level energy
        100,                                // critical agent level energy
        128,                                 // cell_size
        0);                               // timestep
           

    experiment.run();
    
}

void small(){
    
    std::string resultFile = "./../../charts/result.csv";
    std::vector<std::string> mapFilenames, taskFilenames;
    std::vector<_system::TokenType> systemTokenTypes;
    
    mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
    
    taskFilenames.push_back("./Instances/small/kiva-0.2.task");
    taskFilenames.push_back("./Instances/small/kiva-0.5.task");
    taskFilenames.push_back("./Instances/small/kiva-1.task");
    taskFilenames.push_back("./Instances/small/kiva-2.task");
    taskFilenames.push_back("./Instances/small/kiva-5.task");
    taskFilenames.push_back("./Instances/small/kiva-10.task");
    taskFilenames.push_back("./Instances/small/kiva-500.task");
    
    systemTokenTypes.push_back(_system::TokenType::tp);
    systemTokenTypes.push_back(_system::TokenType::taskThreshold_tp);
    systemTokenTypes.push_back(_system::TokenType::carryThreshold_tp);
    systemTokenTypes.push_back(_system::TokenType::taskCarryThreshold_tp);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        taskFilenames,                      // instance tasks
        mapFilenames,                       // instance map
        systemTokenTypes,                   // type of token
        0.3f,                               // task threshold
        0.3f,                               // carry threshold
        6000,                                // current agent level energy
        10000,                                // maximum agent level energy
        6000,                                // charged agent level energy
        1000,                                // critical agent level energy
        32,                                 // cell_size
        0);                               // timestep
           

    experiment.run();
    
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    small();

    return 0;
    
}

