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
#include "_task_path.h"
#include "_ga_estimate_of_path_count.h"
#include "_ga_estimate_of_path_check.h"
#include "_ga_estimate_of_path_collision.h"
#include "_ga_best_solution_selector.h"
#include <string>

using namespace std;


void test(
    int map = 0, 
    std::string of = "*", 
    _ga_solution::EvalType evaltype = _ga_solution::EvalType::balanced, 
    unsigned validity = 1000000){
        
    std::string resultFile =  
            "./results/test_result_" + of 
            + "_" + std::to_string(map)
            + "_" + std::to_string((unsigned)evaltype) 
            + "_" + std::to_string(validity) 
            + ".csv";
    
    unsigned myints[] = {
        1258,7365,77179,201234,
        145,2345,97679,231234,
        1753,9845,77679,591234,
        1685,2345,87679,297234,
        101,3045,67609,899234,
        17,8885,70679,291734};
    
//    unsigned myints[] = {2345};
    
    std::vector<unsigned> seeds (myints, myints + sizeof(myints) / sizeof(unsigned));    
    
    std::vector<std::string> mapFilenames, taskFilenames, tokenIds;   
    std::vector<std::pair<float, float>> thresholds;
    std::vector<_agent_energy_system> agent_energy_systems;
    std::vector<_agentsTasksAllocator*> agentsTasksAllocators;
    
    thresholds.push_back(std::make_pair(.8f, .8f));    
    
    switch(map){
        
        case 1:
            mapFilenames.push_back("./Instances/test/test_1.map");
            break;
        case 2:
            mapFilenames.push_back("./Instances/test/test_2.map");
            break;
        case 3:
            mapFilenames.push_back("./Instances/test/test_3.map");
            break;
        case 4:
            mapFilenames.push_back("./Instances/test/test_4.map");
            break;
        case 5:
            mapFilenames.push_back("./Instances/test/test_5.map");
            break;
        default:
            mapFilenames.push_back("./Instances/test/test_1.map");
            mapFilenames.push_back("./Instances/test/test_2.map");
            mapFilenames.push_back("./Instances/test/test_3.map");
            mapFilenames.push_back("./Instances/test/test_4.map");
            mapFilenames.push_back("./Instances/test/test_5.map");
            
            
    }
        
    taskFilenames.push_back("./Instances/test/test_1.task");
    taskFilenames.push_back("./Instances/test/test_20.task");
    taskFilenames.push_back("./Instances/test/test_82.task");
    taskFilenames.push_back("./Instances/test/test_136.task");
    
    
    _energy_charge<int> ec_1(20000, 20000, 15000, 3000);
    
    _agent_energy_regime aer_1("R_1", -20, 1, 3, 2, 10);
    
    _agent_energy_system aes_1("ES_1", ec_1, aer_1);
    
    agent_energy_systems.push_back(aes_1);
    
    
    _task_path task_path;
    
    _ga_real_of ga_real_of;
    _ga_estimate_of_path_count ga_estimate_of_path_count(task_path, 1, 1, 2);
    _ga_estimate_of_path_check ga_estimate_of_path_check(task_path, 2, 2, 6);
    
    _ga_best_solution_selector select_solution(evaltype);    
//        
    _ga_solutionAllocator ga_solutionAllocator("Greedy", validity);
                    
    _nsga nsga_estimative_path_count(
            "GA_count",
            select_solution,
            ga_estimate_of_path_count,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
//                 return time_span.count() > 5.0 || generation > 99;
                 return generation > 49;
                
            },
            20, 10,  validity, .5f, .5f, .5f, .3f,  .3f, 1);
            
            
    _nsga nsga_estimative_path_check(
            "GA_check",
            select_solution,
            ga_estimate_of_path_check,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
//                 return time_span.count() > 5.0 || generation > 99;
                 return generation > 49;
                
            },
            20, 10,  validity, .5f, .5f, .5f, .3f,  .3f, 1);
                            
    _nsga nsga_real(
            "GA_real",
            select_solution,
            ga_real_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
//                 return time_span.count() > 5.0 || generation > 99;
                 return generation > 49;
                
            },
            20, 10,  validity, .5f, .5f, .5f, .3f,  .3f, 1);
            
    if(of == "G-G"){

        tokenIds.push_back("GAT");        
        agentsTasksAllocators.push_back(&ga_solutionAllocator);     
        
    } else if(of == "GP-G"){
        
        tokenIds.push_back("GAT_P");
        agentsTasksAllocators.push_back(&ga_solutionAllocator);     
        
    } else if(of == "G-E"){
        
        tokenIds.push_back("GAT");
        agentsTasksAllocators.push_back(&nsga_estimative_path_count); 
        agentsTasksAllocators.push_back(&nsga_estimative_path_check);   
        
    } else if(of == "GP-E"){
        
        tokenIds.push_back("GAT_P"); 
        agentsTasksAllocators.push_back(&nsga_estimative_path_count); 
        agentsTasksAllocators.push_back(&nsga_estimative_path_check);   
        
    } else if(of == "G-R"){
        
        tokenIds.push_back("GAT");
        agentsTasksAllocators.push_back(&nsga_real);
        
    } else if(of == "GP-R"){
        
        tokenIds.push_back("GAT_P");
        agentsTasksAllocators.push_back(&nsga_real);
        
    } else if(of == "TP"){
        
        tokenIds.push_back("TP");
        tokenIds.push_back("TSTP");
        
    } else {
            
        tokenIds.push_back("GAT");
        tokenIds.push_back("GAT_P");
        tokenIds.push_back("TP");
        tokenIds.push_back("TSTP");
        agentsTasksAllocators.push_back(&ga_solutionAllocator);
        agentsTasksAllocators.push_back(&nsga_estimative_path_count);
        agentsTasksAllocators.push_back(&nsga_estimative_path_check);
        agentsTasksAllocators.push_back(&nsga_real);   
        
    }
          
    auto experiment = MultiSystemExperiment(
        resultFile,
        seeds,
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

void small(int map = 0, 
    std::string of = "*", 
    _ga_solution::EvalType evaltype = _ga_solution::EvalType::balanced, 
    unsigned validity = 1000000){
    
    std::string resultFile =  
            "./results/small_result_" + of 
            + "_" + std::to_string(map)
            + "_" + std::to_string((unsigned)evaltype) 
            + "_" + std::to_string(validity) 
            + ".csv";
    
    unsigned myints[] = {
        1258,7365,77179,201234,
        145,2345,97679,231234,
        1753,9845,77679,591234,
        1685,2345,87679,297234,
        101,3045,67609,899234,
        17,8885,70679,291734};
    
    std::vector<unsigned> seeds (myints, myints + sizeof(myints) / sizeof(unsigned));
    
    std::vector<std::string> mapFilenames, taskFilenames, tokenIds;
    std::vector<std::pair<float, float>> thresholds;    
    std::vector<_agent_energy_system> agent_energy_systems;
    std::vector<_agentsTasksAllocator*> agentsTasksAllocators;   
    
    thresholds.push_back(std::make_pair(.8f, .8f));
    
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
    
    taskFilenames.push_back("./Instances/small/kiva-0.2.task");
    taskFilenames.push_back("./Instances/small/kiva-0.5.task");
    taskFilenames.push_back("./Instances/small/kiva-1.task");
    taskFilenames.push_back("./Instances/small/kiva-2.task");
    taskFilenames.push_back("./Instances/small/kiva-5.task");
    taskFilenames.push_back("./Instances/small/kiva-10.task");
    taskFilenames.push_back("./Instances/small/kiva-500.task");
    
    
    _energy_charge<int> ec_1(80000000, 80000000, 60000000, 4000000);
    
    _agent_energy_regime aer_1("R_1", -20, 1, 3, 2, 10);
    
    _agent_energy_system aes_1("E_1", ec_1, aer_1);
    
    agent_energy_systems.push_back(aes_1);
    
    _task_path task_path;
    
    _ga_real_of ga_real_of;
    _ga_estimate_of_path_count ga_estimate_of_path_count(task_path, 1, 1, 2);
    _ga_estimate_of_path_check ga_estimate_of_path_check(task_path, 3, 1, 6);
    
    _ga_best_solution_selector select_solution(evaltype);  
        
    _ga_solutionAllocator ga_solutionAllocator("Greedy", validity);
                
    _nsga nsga_estimative_path_count(
            "GA_count",
            select_solution,
            ga_estimate_of_path_count,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                                
//                return time_span.count() > 300.0 || generation > 50;
                return generation > 29;
                
            },  20,   10,  validity, .5f, .5f, .5f,  .3f,  .3f, 1);
            
    _nsga nsga_estimative_path_check(
            "GA_check",
            select_solution,
            ga_estimate_of_path_check,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                                
//                return time_span.count() > 300.0 || generation > 50;
                return generation > 29;
                
            },  20,   10,  validity, .5f, .5f, .5f,  .3f,  .3f, 1);
                            
    _nsga nsga_real(
            "GA_real",
            select_solution,
            ga_real_of,
            [](const _ga_solution& solution, unsigned generation, const std::chrono::duration<double>& time_span){ 
                
                
//                return time_span.count() > 300.0 || generation > 50;
                return generation > 29;
                
            },  20,   10,  validity, .5f, .5f, .5f,  .3f,  .3f, 1);
            
    if(of == "G-G"){

        tokenIds.push_back("GAT");        
        agentsTasksAllocators.push_back(&ga_solutionAllocator);     
        
    } else if(of == "GP-G"){
        
        tokenIds.push_back("GAT_P");
        agentsTasksAllocators.push_back(&ga_solutionAllocator);     
        
    } else if(of == "G-E"){
        
        tokenIds.push_back("GAT");
        agentsTasksAllocators.push_back(&nsga_estimative_path_count); 
        agentsTasksAllocators.push_back(&nsga_estimative_path_check);   
        
    } else if(of == "GP-E"){
        
        tokenIds.push_back("GAT_P"); 
        agentsTasksAllocators.push_back(&nsga_estimative_path_count); 
        agentsTasksAllocators.push_back(&nsga_estimative_path_check);   
        
    } else if(of == "G-R"){
        
        tokenIds.push_back("GAT");
        agentsTasksAllocators.push_back(&nsga_real);
        
    } else if(of == "GP-R"){
        
        tokenIds.push_back("GAT_P");
        agentsTasksAllocators.push_back(&nsga_real);
        
    } else if(of == "TP"){
        
        tokenIds.push_back("TP");
        tokenIds.push_back("TSTP");
        
    } else {
            
        tokenIds.push_back("GAT");
        tokenIds.push_back("GAT_P");
        tokenIds.push_back("TP");
        tokenIds.push_back("TSTP");
        agentsTasksAllocators.push_back(&ga_solutionAllocator);
        agentsTasksAllocators.push_back(&nsga_estimative_path_count);
        agentsTasksAllocators.push_back(&nsga_estimative_path_check);
        agentsTasksAllocators.push_back(&nsga_real);   
        
    }
        
    auto experiment = MultiSystemExperiment(
        resultFile,
        seeds,
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
    
    char* instance = "test";
    char* of = "TP"; // * -> all obj func
    
    unsigned evaltype = 2;
    unsigned validity = 5;    
    int map_index = 0; // 0 -> all maps
    
    if(argc > 1){
        instance = argv[1];
        if(argc > 2){
            of = argv[2];
            if(argc > 3){
                map_index = atoi(argv[3]);
                if(argc > 4){
                    evaltype = atoi(argv[4]);
                    if(argc > 5){
                        validity = atoi(argv[5]);
                    }
                }
            }
        }
    }
    
    std::string str(instance);
    std::string ofs(of);
    
    if(str == "test"){
        
        test(map_index, ofs, (_ga_solution::EvalType)evaltype, validity);
        
    }else if(str == "small"){
        
        small(map_index, ofs, (_ga_solution::EvalType)evaltype, validity);
        
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

