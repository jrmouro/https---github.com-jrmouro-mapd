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
    
    std::string resultFile = "./../../charts/test_result.csv";
    std::vector<std::string> mapFilenames, taskFilenames;
    std::vector<_system::TokenType> systemTokenTypes;    
    std::vector<std::pair<float, float>> thresholds;
    
    thresholds.push_back(std::make_pair(1.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.6f, .6f));
//    thresholds.push_back(std::make_pair(.4f, .4f));
//    thresholds.push_back(std::make_pair(.4f, .8f));
    
//    mapFilenames.push_back("./Instances/test/test_2.map");
    mapFilenames.push_back("./Instances/test/test_4.map");
//    mapFilenames.push_back("./Instances/test/test_6.map");
//    mapFilenames.push_back("./Instances/test/test_8.map");
    
    taskFilenames.push_back("./Instances/test/test_1.task");
//    taskFilenames.push_back("./Instances/test/test_27.task");
//    taskFilenames.push_back("./Instances/test/test_79.task");
    
//    systemTokenTypes.push_back(_system::TokenType::tokenPass);
//    systemTokenTypes.push_back(_system::TokenType::threshold_tokenPass);
    systemTokenTypes.push_back(_system::TokenType::backwardTask_tokenPass);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        thresholds,
        taskFilenames,                      // instance tasks
        mapFilenames,                       // instance map
        systemTokenTypes,                   // type of token
        800,                                // current agent level energy
        1000,                                // maximum agent level energy
        800,                                // charged agent level energy
        500,                                // critical agent level energy
        128,                                 // cell_size
        1000);                               // timestep
           

    experiment.run();
    
    _updateTokenAlgorithms::deleteInstance();
    
}

void small(){
    
    std::string resultFile = "./../../charts/small_result.csv";
    std::vector<std::string> mapFilenames, taskFilenames;
    std::vector<_system::TokenType> systemTokenTypes;
    std::vector<std::pair<float, float>> thresholds;
    
    thresholds.push_back(std::make_pair(1.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.7f, .7f));
    thresholds.push_back(std::make_pair(.5f, .5f));
//    thresholds.push_back(std::make_pair(.3f, .5f));
    
    mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
//    
    taskFilenames.push_back("./Instances/small/kiva-0.2.task");
    taskFilenames.push_back("./Instances/small/kiva-0.5.task");
    taskFilenames.push_back("./Instances/small/kiva-1.task");
    taskFilenames.push_back("./Instances/small/kiva-2.task");
    taskFilenames.push_back("./Instances/small/kiva-5.task");
    taskFilenames.push_back("./Instances/small/kiva-10.task");
    taskFilenames.push_back("./Instances/small/kiva-500.task");
    
    systemTokenTypes.push_back(_system::TokenType::tokenPass);
    systemTokenTypes.push_back(_system::TokenType::backwardTask_tokenPass);
    systemTokenTypes.push_back(_system::TokenType::threshold_tokenPass);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        thresholds,
        taskFilenames,                      // instance tasks
        mapFilenames,                       // instance map
        systemTokenTypes,                   // type of token
        2000,                                // current agent level energy
        3000,                                // maximum agent level energy
        2000,                                // charged agent level energy
        1000,                                // critical agent level energy
        45,                                 // cell_size
        0);                               // timestep           

    experiment.run();    
    
    _updateTokenAlgorithms::deleteInstance();
    
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    small();

    return 0;
    
}

