/* 
 * File:   main.cpp
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:31
 */

#include "MultiSystemExperiment.h"
#include "_updateTokenAlgorithms.h"
#include "_agent_energy_system.h"
#include "_energy_charge.h"
#include "_agentsTasksAllocator.h"
#include "_ga_solutionAllocator.h"

using namespace std;

void test(){
    
    std::string resultFile = "./../../charts/test_result.csv";
    std::vector<std::string> mapFilenames, taskFilenames, tokenIds;   
    std::vector<std::pair<float, float>> thresholds;
    std::vector<_agent_energy_system> agent_energy_systems;
    std::vector<_agentsTasksAllocator*> agentsTasksAllocators;
    
    thresholds.push_back(std::make_pair(.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.7f, .7f));
//    thresholds.push_back(std::make_pair(.5f, .5f));
//    thresholds.push_back(std::make_pair(.3f, .5f));
    
    mapFilenames.push_back("./Instances/test/test_2.map");
//    mapFilenames.push_back("./Instances/test/test_3.map");
//    mapFilenames.push_back("./Instances/test/test_4.map");  
    
    taskFilenames.push_back("./Instances/test/ga_test_1.task");
    
//    tokenIds.push_back("TP");
//    tokenIds.push_back("TSTP");
//    tokenIds.push_back("BTT");

    
    _energy_charge<int> ec_1(200, 200, 150, 30);
    _energy_charge<int> ec_2(2000, 2000, 1500, 300);
    
    _agent_energy_regime aer_1("R_1", -20, 1, 3, 2, 10);
    _agent_energy_regime aer_2("R_2", -100, 1, 3, 2, 100);
    
    _agent_energy_system aes_1("ES_1", ec_1, aer_1);
    _agent_energy_system aes_2("ES_2", ec_2, aer_2);
    
    agent_energy_systems.push_back(aes_1);
//    agent_energy_systems.push_back(aes_2);
    
    tokenIds.push_back("GA");
    _ga_solutionAllocator ga_solutionAllocator;
    agentsTasksAllocators.push_back(&ga_solutionAllocator);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        tokenIds,
        thresholds,
        taskFilenames,                      
        mapFilenames,                      
        agent_energy_systems,              
        agentsTasksAllocators,
        500,
        80,                               
        0);                              
           

    experiment.run();
    
    _updateTokenAlgorithms::deleteInstance();
    
}

void small(){
    
    std::string resultFile = "./../../charts/small_result.csv";
    std::vector<std::string> mapFilenames, taskFilenames, tokenIds;
    std::vector<std::pair<float, float>> thresholds;    
    std::vector<_agent_energy_system> agent_energy_systems;
    std::vector<_agentsTasksAllocator*> agentsTasksAllocators;
//    
    thresholds.push_back(std::make_pair(.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.7f, .7f));
//    thresholds.push_back(std::make_pair(.5f, .5f));
//    thresholds.push_back(std::make_pair(.3f, .3f));
//    
    mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
    mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
////    
    taskFilenames.push_back("./Instances/small/kiva-0.2.task");
    taskFilenames.push_back("./Instances/small/kiva-0.5.task");
    taskFilenames.push_back("./Instances/small/kiva-1.task");
    taskFilenames.push_back("./Instances/small/kiva-2.task");
    taskFilenames.push_back("./Instances/small/kiva-5.task");
    taskFilenames.push_back("./Instances/small/kiva-10.task");
    taskFilenames.push_back("./Instances/small/kiva-500.task");
    
//    tokenIds.push_back("TP");
//    tokenIds.push_back("TSTP");
//    tokenIds.push_back("BTT");
//    tokenIds.push_back("TTP");
    
    _energy_charge<int> ec_1(8000, 8000, 6000, 400);
    _energy_charge<int> ec_2(80000, 80000, 60000, 4000);
    _energy_charge<int> ec_3(800000, 800000, 600000, 40000);
    
    _agent_energy_regime aer_1("R_1", -10, 1, 1, 1, 1);
    _agent_energy_regime aer_2("R_2", -10, 1, 1, 1, 10);
    _agent_energy_regime aer_3("R_3", -10, 1, 1, 1, 100);
    
    _agent_energy_system aes_1("E_1", ec_1, aer_1);
    _agent_energy_system aes_2("E_2", ec_2, aer_2);
    _agent_energy_system aes_3("E_3", ec_3, aer_3);
    
    agent_energy_systems.push_back(aes_1);
//    agent_energy_systems.push_back(aes_2);
//    agent_energy_systems.push_back(aes_3);
    
    tokenIds.push_back("GA");
    _ga_solutionAllocator ga_solutionAllocator;
    agentsTasksAllocators.push_back(&ga_solutionAllocator);
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        tokenIds,
        thresholds,
        taskFilenames,                      
        mapFilenames,                      
        agent_energy_systems,  
        agentsTasksAllocators,
        500,
        46,                               
        100);                              
           
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

