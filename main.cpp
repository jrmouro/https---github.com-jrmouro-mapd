/* 
 * File:   main.cpp
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:31
 */

#include "MultiSystemExperiment.h"
#include "_updateTokenAlgorithms.h"

using namespace std;

void test(){
    
    std::string resultFile = "./../../charts/result.csv";
    std::vector<std::string> mapFilenames, taskFilenames;
    std::vector<_system::TokenType> systemTokenTypes;    
    std::vector<std::pair<float, float>> thresholds;
    
    thresholds.push_back(std::make_pair(1.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.6f, .6f));
//    thresholds.push_back(std::make_pair(.4f, .4f));
    thresholds.push_back(std::make_pair(.4f, .8f));
    
//    mapFilenames.push_back("./Instances/test/test_0.map");
    mapFilenames.push_back("./Instances/test/test_1.map");
    mapFilenames.push_back("./Instances/test/test_2.map");
    
//    taskFilenames.push_back("./Instances/test/test_0.task");
    taskFilenames.push_back("./Instances/test/test_1.task");
    taskFilenames.push_back("./Instances/test/test_2.task");
    
//    systemTokenTypes.push_back(_system::TokenType::tp);
//    systemTokenTypes.push_back(_system::TokenType::tokenPass);
    systemTokenTypes.push_back(_system::TokenType::threshold_tokenPass);
//    systemTokenTypes.push_back(_system::TokenType::taskCarryThreshold_tp);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        thresholds,
        taskFilenames,                      // instance tasks
        mapFilenames,                       // instance map
        systemTokenTypes,                   // type of token
        600,                                // current agent level energy
        1000,                                // maximum agent level energy
        600,                                // charged agent level energy
        100,                                // critical agent level energy
        64,                                 // cell_size
        10);                               // timestep
           

    experiment.run();
    
    _updateTokenAlgorithms::deleteInstance();
    
}

void small(){
    
    std::string resultFile = "./../../charts/result.csv";
    std::vector<std::string> mapFilenames, taskFilenames;
    std::vector<_system::TokenType> systemTokenTypes;
    std::vector<std::pair<float, float>> thresholds;
    
    thresholds.push_back(std::make_pair(.8f, .8f));
//    thresholds.push_back(std::make_pair(.6f, .6f));
    thresholds.push_back(std::make_pair(.4f, .4f));
    thresholds.push_back(std::make_pair(.4f, .8f));
    
    mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
//    mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
//    mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
//    mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
//    mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
    
//    taskFilenames.push_back("./Instances/small/kiva-0.2.task");
    taskFilenames.push_back("./Instances/small/kiva-0.5.task");
//    taskFilenames.push_back("./Instances/small/kiva-1.task");
//    taskFilenames.push_back("./Instances/small/kiva-2.task");
//    taskFilenames.push_back("./Instances/small/kiva-5.task");
//    taskFilenames.push_back("./Instances/small/kiva-10.task");
//    taskFilenames.push_back("./Instances/small/kiva-500.task");
    
    systemTokenTypes.push_back(_system::TokenType::tokenPass);
//    systemTokenTypes.push_back(_system::TokenType::carryThreshold_tp);
//    systemTokenTypes.push_back(_system::TokenType::threshold_tokenPass);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        thresholds,
        taskFilenames,                      // instance tasks
        mapFilenames,                       // instance map
        systemTokenTypes,                   // type of token
        300000,                                // current agent level energy
        300000,                                // maximum agent level energy
        250000,                                // charged agent level energy
        10000,                                // critical agent level energy
        32,                                 // cell_size
        50);                               // timestep
           

    experiment.run();    
    
    _updateTokenAlgorithms::deleteInstance();
    
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    test();

    return 0;
    
}

