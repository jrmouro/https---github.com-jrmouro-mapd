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
#include "_ga_balanced_solution.h"
#include "_nsga.h"
#include "_ga_real_of.h"
#include "_ga_estimate_of.h"
#include "_ga_estimate_of_path.h"
#include <string>

using namespace std;

void bug(int map = 0, std::string of = "*"){
    
    std::string resultFile = "./results/bug_result_" + of + "_" + std::to_string(map) + ".csv";
    std::vector<std::string> mapFilenames, taskFilenames, tokenIds;
    std::vector<std::pair<float, float>> thresholds;    
    std::vector<_agent_energy_system> agent_energy_systems;
    std::vector<_agentsTasksAllocator*> agentsTasksAllocators;
//    
    thresholds.push_back(std::make_pair(.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.7f, .7f));
//    thresholds.push_back(std::make_pair(.5f, .5f));
//    thresholds.push_back(std::make_pair(.3f, .3f));
    
    switch(map){
        case 1:
            mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
            break;
        case 2:
            mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
            break;
        case 3:
            mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
            break;
        case 4:
            mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
        break;
        case 5:
            mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
        break;
        default:
            mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
    }
//    
////    
    taskFilenames.push_back("./Instances/small/kiva-0.2.task");
    taskFilenames.push_back("./Instances/small/kiva-0.5.task");
    taskFilenames.push_back("./Instances/small/kiva-1.task");
    taskFilenames.push_back("./Instances/small/kiva-2.task");
    taskFilenames.push_back("./Instances/small/kiva-5.task");
    taskFilenames.push_back("./Instances/small/kiva-10.task");
    taskFilenames.push_back("./Instances/small/kiva-500.task");
    
    tokenIds.push_back("TP");
//    tokenIds.push_back("TSTP");
//    tokenIds.push_back("BTT");
//    tokenIds.push_back("TTP");
    
    _energy_charge<int> ec_1(8000000, 8000000, 6000000, 400000);
//    _energy_charge<int> ec_2(80000, 80000, 60000, 4000);
//    _energy_charge<int> ec_3(800000, 800000, 600000, 40000);
    
    _agent_energy_regime aer_1("R_1", -10, 1, 1, 1, 1);
//    _agent_energy_regime aer_2("R_2", -10, 1, 1, 1, 10);
//    _agent_energy_regime aer_3("R_3", -10, 1, 1, 1, 100);
    
    _agent_energy_system aes_1("E_1", ec_1, aer_1);
//    _agent_energy_system aes_2("E_2", ec_2, aer_2);
//    _agent_energy_system aes_3("E_3", ec_3, aer_3);
    
    agent_energy_systems.push_back(aes_1);
//    agent_energy_systems.push_back(aes_2);
//    agent_energy_systems.push_back(aes_3);
    
    _ga_real_of ga_real_of;
    _ga_estimate_of ga_estimate_of;     
    _ga_estimate_of_path ga_estimate_of_path; 
    
    _ga_balanced_solution select_solution;
    
//    tokenIds.push_back("GAT");
//    tokenIds.push_back("GAT_P");
    
    _ga_solutionAllocator ga_solutionAllocator("Greedy", 25000);
    
    _nsga nsga_estimative(
            "NSGA-II_e",
            select_solution,
            ga_estimate_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                                
//                return time_span.count() > 5.0 || generation > 29;
                return generation > 5;
                
            },  30,   10,  25000, .4f, .5f, .5f,  .3f,  .3f, 1);
            
    _nsga nsga_estimative_path(
            "NSGA-II_e_p",
            select_solution,
            ga_estimate_of_path,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                                
//                return time_span.count() > 5.0 || generation > 29;
                return generation > 5;
                
            },  30,   10,  25000, .4f, .5f, .5f,  .3f,  .3f, 1);
            
    _nsga nsga_real(
            "NSGA-II_r",
            select_solution,
            ga_real_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
                
//                return time_span.count() > 5.0 || generation > 29;
                return generation > 5;
                
            },  30,   10,  25000, .4f, .5f, .5f,  .3f,  .3f, 1);
            
    if(of == "g"){

        agentsTasksAllocators.push_back(&ga_solutionAllocator);     
        
    } else if(of == "e"){
        
        agentsTasksAllocators.push_back(&nsga_estimative);    
        
    } else if(of == "p"){
        
        agentsTasksAllocators.push_back(&nsga_estimative_path);    
        
    } else if(of == "ep"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator); 
        agentsTasksAllocators.push_back(&nsga_estimative); 
        agentsTasksAllocators.push_back(&nsga_estimative_path);     
        
    } else if(of == "r"){
        
        agentsTasksAllocators.push_back(&nsga_real);
        
    } else {
                
        agentsTasksAllocators.push_back(&ga_solutionAllocator);
        agentsTasksAllocators.push_back(&nsga_estimative);
        agentsTasksAllocators.push_back(&nsga_estimative_path); 
        agentsTasksAllocators.push_back(&nsga_real);   
        
    }
        
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
        0);                              
           
    experiment.run();
    
    _updateTokenAlgorithms::deleteInstance();
    
}

void test(int map = 0, std::string of = "e"){
    
    std::string resultFile = "./results/test_result_" + of + "_" + std::to_string(map) + ".csv";
    std::vector<std::string> mapFilenames, taskFilenames, tokenIds;   
    std::vector<std::pair<float, float>> thresholds;
    std::vector<_agent_energy_system> agent_energy_systems;
    std::vector<_agentsTasksAllocator*> agentsTasksAllocators;
    
    thresholds.push_back(std::make_pair(.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.7f, .7f));
//    thresholds.push_back(std::make_pair(.5f, .5f));
//    thresholds.push_back(std::make_pair(.3f, .5f));
    
    
    switch(map){
        case 1:
            mapFilenames.push_back("./Instances/test/test_2.map");
            break;
        case 2:
            mapFilenames.push_back("./Instances/test/test_3.map");
            break;
        case 3:
            mapFilenames.push_back("./Instances/test/test_4.map");
            break;
        default:
            mapFilenames.push_back("./Instances/test/test_2.map");
            mapFilenames.push_back("./Instances/test/test_3.map");
            mapFilenames.push_back("./Instances/test/test_4.map");
            ;
    }
    
    
//    taskFilenames.push_back("./Instances/test/ga_test_1.task");
    
    
    taskFilenames.push_back("./Instances/test/test_1.task");
    taskFilenames.push_back("./Instances/test/test_20.task");
    taskFilenames.push_back("./Instances/test/test_82.task");
    taskFilenames.push_back("./Instances/test/test_136.task");
    
    tokenIds.push_back("TP");
//    tokenIds.push_back("TSTP");
//    tokenIds.push_back("BTT");

    
    _energy_charge<int> ec_1(20000, 20000, 15000, 3000);
    _energy_charge<int> ec_2(2000, 2000, 1500, 300);
    
    _agent_energy_regime aer_1("R_1", -20, 1, 3, 2, 10);
    _agent_energy_regime aer_2("R_2", -100, 1, 3, 2, 100);
    
    _agent_energy_system aes_1("ES_1", ec_1, aer_1);
    _agent_energy_system aes_2("ES_2", ec_2, aer_2);
    
    agent_energy_systems.push_back(aes_1);
//    agent_energy_systems.push_back(aes_2);
    
    _ga_real_of ga_real_of;
    _ga_estimate_of ga_estimate_of; 
    _ga_estimate_of_path ga_estimate_of_path; 
    _ga_balanced_solution select_solution;    
//    
//    tokenIds.push_back("GAT");
//    tokenIds.push_back("GAT_P");
    
    _ga_solutionAllocator ga_solutionAllocator("Greedy", 25000);
    
    _nsga nsga_estimative(
            "NSGA_II_e",
            select_solution,
            ga_estimate_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
//                 return time_span.count() > 5.0 || generation > 99;
                 return generation > 100;
                
            },
            30, 10,  25000, .4f, .5f, .5f, .3f,  .3f, 1);
            
    _nsga nsga_estimative_path(
            "NSGA_II_e_p",
            select_solution,
            ga_estimate_of_path,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
//                 return time_span.count() > 5.0 || generation > 99;
                 return generation > 100;
                
            },
            30, 10,  25000, .4f, .5f, .5f, .3f,  .3f, 1);
            
            
    _nsga nsga_real(
            "NSGA_II_r",
            select_solution,
            ga_real_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
//                 return time_span.count() > 5.0 || generation > 99;
                 return generation > 100;
                
            },
            30, 10,  25000, .4f, .5f, .5f, .3f,  .3f, 1);
            
    if(of == "g"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator);     
        
    } else if(of == "e"){
        
        agentsTasksAllocators.push_back(&nsga_estimative);    
        
    } else if(of == "p"){
        
        agentsTasksAllocators.push_back(&nsga_estimative_path);    
        
    } else if(of == "ep"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator); 
        agentsTasksAllocators.push_back(&nsga_estimative); 
        agentsTasksAllocators.push_back(&nsga_estimative_path);     
        
    } else if(of == "r"){
        
        agentsTasksAllocators.push_back(&nsga_real);
        
    } else {
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator);
        agentsTasksAllocators.push_back(&nsga_estimative);
        agentsTasksAllocators.push_back(&nsga_estimative_path);
        agentsTasksAllocators.push_back(&nsga_real);                
        
    }
          
    auto experiment = MultiSystemExperiment(
        resultFile,
        tokenIds,
        thresholds,
        taskFilenames,                      
        mapFilenames,                      
        agent_energy_systems,              
        agentsTasksAllocators,
        500,
        60,                               
        0);                              
           
    experiment.run();
    
    _updateTokenAlgorithms::deleteInstance();
    
}

void small(int map, std::string of = "e"){
    
    std::string resultFile = "./results/small_result_" + of + "_" + std::to_string(map) + ".csv";
    std::vector<std::string> mapFilenames, taskFilenames, tokenIds;
    std::vector<std::pair<float, float>> thresholds;    
    std::vector<_agent_energy_system> agent_energy_systems;
    std::vector<_agentsTasksAllocator*> agentsTasksAllocators;
//    
    thresholds.push_back(std::make_pair(.0f, 1.0f));
//    thresholds.push_back(std::make_pair(.7f, .7f));
//    thresholds.push_back(std::make_pair(.5f, .5f));
//    thresholds.push_back(std::make_pair(.3f, .3f));
    
    switch(map){
        case 1:
            mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
            break;
        case 2:
            mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
            break;
        case 3:
            mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
            break;
        case 4:
            mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
        break;
        case 5:
            mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
        break;
        default:
            mapFilenames.push_back("./Instances/small/kiva-10-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-20-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-30-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-40-500-5.map");
            mapFilenames.push_back("./Instances/small/kiva-50-500-5.map");
    }
//    
////    
    taskFilenames.push_back("./Instances/small/kiva-0.2.task");
    taskFilenames.push_back("./Instances/small/kiva-0.5.task");
    taskFilenames.push_back("./Instances/small/kiva-1.task");
    taskFilenames.push_back("./Instances/small/kiva-2.task");
    taskFilenames.push_back("./Instances/small/kiva-5.task");
    taskFilenames.push_back("./Instances/small/kiva-10.task");
    taskFilenames.push_back("./Instances/small/kiva-500.task");
    
    tokenIds.push_back("TP");
//    tokenIds.push_back("TSTP");
//    tokenIds.push_back("BTT");
//    tokenIds.push_back("TTP");
    
    _energy_charge<int> ec_1(8000000, 8000000, 6000000, 400000);
//    _energy_charge<int> ec_2(80000, 80000, 60000, 4000);
//    _energy_charge<int> ec_3(800000, 800000, 600000, 40000);
    
    _agent_energy_regime aer_1("R_1", -10, 1, 1, 1, 1);
//    _agent_energy_regime aer_2("R_2", -10, 1, 1, 1, 10);
//    _agent_energy_regime aer_3("R_3", -10, 1, 1, 1, 100);
    
    _agent_energy_system aes_1("E_1", ec_1, aer_1);
//    _agent_energy_system aes_2("E_2", ec_2, aer_2);
//    _agent_energy_system aes_3("E_3", ec_3, aer_3);
    
    agent_energy_systems.push_back(aes_1);
//    agent_energy_systems.push_back(aes_2);
//    agent_energy_systems.push_back(aes_3);
    
    _ga_real_of ga_real_of;
    _ga_estimate_of ga_estimate_of; 
    _ga_estimate_of ga_estimate_of_path;
    _ga_balanced_solution select_solution;
    
    tokenIds.push_back("GAT");
    tokenIds.push_back("GAT_P");
    
    _ga_solutionAllocator ga_solutionAllocator("Greedy", 25000);
    
    _nsga nsga_estimative(
            "NSGA-II_e",
            select_solution,
            ga_estimate_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                                
//                return time_span.count() > 5.0 || generation > 29;
                return generation > 30;
                
            },  30,   10,  25000, .4f, .5f, .5f,  .3f,  .3f, 1);
            
    _nsga nsga_estimative_path(
            "NSGA-II_e_p",
            select_solution,
            ga_estimate_of_path,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                                
//                return time_span.count() > 5.0 || generation > 29;
                return generation > 30;
                
            },  30,   10,  25000, .4f, .5f, .5f,  .3f,  .3f, 1);
            
    _nsga nsga_real(
            "NSGA-II_r",
            select_solution,
            ga_real_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
                
//                return time_span.count() > 5.0 || generation > 29;
                return generation > 30;
                
            },  30,   10,  25000, .4f, .5f, .5f,  .3f,  .3f, 1);
            
    if(of == "g"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator);     
        
    } else if(of == "e"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator); 
        agentsTasksAllocators.push_back(&nsga_estimative);     
        
    } else if(of == "p"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator); 
        agentsTasksAllocators.push_back(&nsga_estimative_path);     
        
    } else if(of == "ep"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator); 
        agentsTasksAllocators.push_back(&nsga_estimative); 
        agentsTasksAllocators.push_back(&nsga_estimative_path);     
        
    } else if(of == "r"){
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator); 
        agentsTasksAllocators.push_back(&nsga_real);
        
    } else {
        
        agentsTasksAllocators.push_back(&ga_solutionAllocator);   
        agentsTasksAllocators.push_back(&nsga_estimative);
        agentsTasksAllocators.push_back(&nsga_estimative_path); 
        agentsTasksAllocators.push_back(&nsga_real);             
        
    }
        
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
        0);                              
           
    experiment.run();
    
    _updateTokenAlgorithms::deleteInstance();
    
    
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    char* instance = "small";
    char* of = "ep"; // * -> all obj func
    
    int map_index = 0; // 0 -> all maps
    
    if(argc > 1){
        instance = argv[1];
        if(argc > 2){
            of = argv[2];
            if(argc > 3){
                map_index = atoi(argv[3]);
            }
        }
    }
    
    std::string str(instance);
    std::string ofs(of);
    
    if(str == "bug"){
        
        bug(map_index, ofs);
        
    } else if(str == "test"){
        
        test(map_index, ofs);
        
    }else if(str == "small"){
        
        small(map_index, ofs);
        
    }else{        
    
        try {
            std::ostringstream stream;
            stream << "invalid instance group name: " << std::string(instance);
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
    }

    return 0;
    
}

