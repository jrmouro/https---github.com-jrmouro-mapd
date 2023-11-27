/* 
 * File:   MultiSystemExperiment.h
 * Author: ronaldo
 *
 * Created on 31 de agosto de 2023, 04:45
 */

#ifndef MULTISYSTEMEXPERIMENT_H
#define MULTISYSTEMEXPERIMENT_H

#include <set>
#include "SystemExperiment.h"
#include "GA_SystemExperiment.h"
#include "ThresholdTokenPass.h"
#include "TokenPass.h"
#include "TaskSwapTokenPass.h"
#include "BackwardTaskToken.h"
#include "_agentsTasksAllocator.h"
#include "_ga_token_p.h"


class MultiSystemExperiment : public Experiment<std::string>{
    
public:
    
    MultiSystemExperiment(
            const std::string& resultFilename,
            const std::vector<unsigned>& seeds,
            const std::vector<std::string>& tokenIds,
            const std::vector<std::pair<float, float>>& thresholds,
            const std::vector<std::string>& taskFilenames,             
            const std::vector<std::string>& mapFilenames,
            const std::vector<_agent_energy_system>& agent_energy_systems,
            const std::vector<_agentsTasksAllocator*>& agentsTasksAllocators,
            const unsigned task_threshold,
            const unsigned cell_size = 0, 
            const unsigned timestep = 0) :
                    Experiment<std::string>("MultiSystemExperiment"),
                    resultFilename(resultFilename),
                    seeds(seeds),
                    thresholds(thresholds),
                    mapFilenames(mapFilenames), 
                    taskFilenames(taskFilenames), 
                    tokenIds(tokenIds), 
                    agent_energy_systems(agent_energy_systems),
                    agentsTasksAllocators(agentsTasksAllocators),
                    task_threshold(task_threshold),
                    cell_size(cell_size), 
                    timestep(timestep) {}

    MultiSystemExperiment(const MultiSystemExperiment& other) :
            Experiment<std::string>(other),
            resultFilename(other.resultFilename),
            seeds(other.seeds),
            thresholds(other.thresholds),
            mapFilenames(other.mapFilenames), 
            taskFilenames(other.taskFilenames), 
            tokenIds(other.tokenIds), 
            agent_energy_systems(other.agent_energy_systems),
            agentsTasksAllocators(other.agentsTasksAllocators),
            task_threshold(other.task_threshold),
            cell_size(other.cell_size), 
            timestep(other.timestep) { }

    virtual void run(){
        
        bool headerFlag = true;
        
        std::ofstream ofs (resultFilename, std::ofstream::out);        
        
        for (auto const& mapFilename : mapFilenames) {
            
            auto imap = InstanceMap::load(mapFilename);
                        
            for (auto const& taskFilename : taskFilenames) {
                
                _map map(imap->getMap());
                
                auto itasks = InstanceTask::load(taskFilename, [&map](unsigned id){
                    return *map.getNoBotEndPointById(id);
                });
                
                itasks->getTaskMap().listTasks([&map](unsigned step, const _task& task){  
                    map.setTypeOfSite(task.getPickup().GetRow(), task.getPickup().GetColunm(), _map::TypeOfSite::task);
                    map.setTypeOfSite(task.getDelivery().GetRow(), task.getDelivery().GetColunm(), _map::TypeOfSite::task);           
                    return false;
                });
                
                
//                imap->resetTaskEndpoint();
                
//                auto itasks = InstanceTask::load(taskFilename, [&imap](unsigned id){
//                    return *imap->getMap().getNoBotEndPointById(id);
//                });
//
//                itasks->getTaskMap().listTasks([&imap](unsigned step, const _task& task){       
//                    imap->setTaskEndpoint(task.getPickup().GetRow(), task.getPickup().GetColunm());
//                    imap->setTaskEndpoint(task.getDelivery().GetRow(), task.getDelivery().GetColunm());            
//                    return false;
//                });
                
                std::vector<_token*> ptokens;
                std::vector<_ga_token*> p_ga_tokens;
                
                for (auto const& tokenId : tokenIds) {
                    
                    if(tokenId == "GAT"){
                        
                        for (auto agent_energy_system : agent_energy_systems) {
                            
                            p_ga_tokens.push_back(new _ga_token(map, imap->getStepMap(), agent_energy_system));

                        }
                    
                    } else if(tokenId == "GAT_P"){
                        
                        for (auto agent_energy_system : agent_energy_systems) {
                            
                            p_ga_tokens.push_back(new _ga_token_p(map, imap->getStepMap(), agent_energy_system));

                        }
                    
                    } else if(tokenId == "TP") {
                        
                        for (auto agent_energy_system : agent_energy_systems) {
                            
                            ptokens.push_back(new TokenPass(map, imap->getStepMap(), agent_energy_system, task_threshold));

                        }
                        
                    } else if(tokenId == "TSTP") {
                        
                        for (auto agent_energy_system : agent_energy_systems) {
                            
                            ptokens.push_back(new TaskSwapTokenPass(map, imap->getStepMap(), agent_energy_system, task_threshold));

                        }
                        
                    }  else if(tokenId == "BTT") {
                        
                        std::set<float> deliveryThresholdSet;                        
                        
                        for (auto threshold : thresholds) {
                            
                            std::set<float>::iterator it = deliveryThresholdSet.find(threshold.second);
                            
                            if(it == deliveryThresholdSet.end()){
                                
                                deliveryThresholdSet.insert(threshold.second);
                            
                                for (auto agent_energy_system : agent_energy_systems) {

                                    ptokens.push_back(new BackwardTaskToken(map, imap->getStepMap(), agent_energy_system, threshold.second, task_threshold));

                                }
                            
                            }

                        }
                                          
                        
                    }   else if(tokenId == "TTP") {
                        
                        for (auto threshold : thresholds) {
                            
                            for (auto agent_energy_system : agent_energy_systems) {                               
                                
                                ptokens.push_back(new ThresholdTokenPass(map, imap->getStepMap(), agent_energy_system, threshold.first, threshold.second, task_threshold));

                            }

                        }
                        
                    }        

                }
                
                for (auto ptoken : p_ga_tokens) { 
                    
                    for (auto agentsTasksAllocator : agentsTasksAllocators) {
                        
                        for(auto const& seed: seeds){
                        
//                        auto allocatorClone = agentsTasksAllocator->emptyClone();
                            
                            agentsTasksAllocator->setSeed(seed);
                        
                            _ga_token* token = ptoken->getClone();

                            token->setName(token->id() + "("+ agentsTasksAllocator->id() + ")");

                            GA_SystemExperiment se(
                                        token->getName() + ";" + mapFilename + " ;" + taskFilename,
                                        *agentsTasksAllocator,
                                        *token,  
                                        itasks->getTaskMap(),
                                        cell_size,
                                        timestep);  



        //                    std::cout << " - token: " << *ptoken << std::endl;
                            std::cout << "GA System Experiment: " << std::endl;
                            std::cout << " - id: " << se.id() << std::endl;
                            std::cout << " - energy system: " << token->getAgent_energy_system().id() << std::endl;

                            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

                            se.run();

                            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

                            std::cout << " - makespan: " << token->getCurrentStep() << std::endl;
                            std::cout << " - energy expenditure: " << token->energyExpenditure() << std::endl;
                            std::cout << " - pending tasks: " << token->getPendingTaskAmount() << std::endl;
                            std::cout << " - assigned tasks: " << token->getAssignedTaskAmount() << std::endl;
                            std::cout << " - running tasks: " << token->getRunningTaskAmount() << std::endl;
                            std::cout << " - finished tasks: " << token->getFinishedTaskAmount() << std::endl;
                            std::cout << " - duration: " << time_span.count() << " seconds." << std::endl;
                            std::cout << " - seed: " <<  agentsTasksAllocator->getSeed() << std::endl << std::endl;
        //                    std::cout << " - report: " << std::endl << se.getToken().getReportTaskMap() <<  std::endl << std::endl;
        //                    std::cout << " - token: " << *ptoken << std::endl;

                            if(headerFlag){

                                headerFlag = false;

                                Writable::strWrite(*imap, ofs, "mapFilename", true);
                                Writable::strWrite(*imap, ofs, "taskFilename", true); 
                                imap->writeHeader(ofs);
                                Writable::sepWrite(*imap, ofs);
                                itasks->writeHeader(ofs);
                                Writable::sepWrite(*imap, ofs);
                                token->writeHeader(ofs);
                                Writable::sepWrite(*imap, ofs);
                                Writable::strWrite(*imap, ofs, "time(s)", true);
                                Writable::endlWrite(*imap, ofs);

                            }

                            Writable::strWrite(*imap, ofs, mapFilename, true);
                            Writable::strWrite(*imap, ofs, taskFilename, true);
                            imap->writeRow(ofs);
                            Writable::sepWrite(*imap, ofs);
                            itasks->writeRow(ofs);
                            Writable::sepWrite(*imap, ofs);
                            token->writeRow(ofs);
                            Writable::sepWrite(*imap, ofs);
                            Writable::strWrite(*imap, ofs, std::to_string(time_span.count()), true);
                            Writable::endlWrite(*imap, ofs); 

    //                        delete allocatorClone;

                            delete token;
                            
                            if(!agentsTasksAllocator->isStochastic()) break;

                        }
                        
                    }

                    delete ptoken;
                    
                }
                
                for (auto ptoken : ptokens) {                   
                    
                    SystemExperiment se(
                                    ptoken->name() + " ;" + mapFilename + " ;" + taskFilename,
                                    itasks->getTaskMap(), 
                                    *ptoken,           
                                    cell_size,
                                    timestep);  
                    
//                    std::cout << " - token: " << *ptoken << std::endl;
                    std::cout << "System Experiment: " << std::endl;
                    std::cout << " - id: " << se.id() << std::endl;
                    std::cout << " - energy system: " << ptoken->getAgent_energy_system().id() << std::endl;
                    
                    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

                    se.run();
                    
                    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
                    
                    std::cout << " - makespan: " << ptoken->getCurrentStep() << std::endl;
                    std::cout << " - energy expenditure: " << ptoken->energyExpenditure() << std::endl;
                    std::cout << " - pending tasks: " << ptoken->getPendingTaskAmount() << std::endl;
                    std::cout << " - assigned tasks: " << ptoken->getAssignedTaskAmount() << std::endl;
                    std::cout << " - running tasks: " << ptoken->getRunningTaskAmount() << std::endl;
                    std::cout << " - baskward tasks: " << ptoken->getBackwardTaskAmount() << std::endl;
                    std::cout << " - finished tasks: " << ptoken->getFinishedTaskAmount() << std::endl;
                    std::cout << " - duration: " << time_span.count() << " seconds." << std::endl << std::endl; 
//                    std::cout << " - report: " << std::endl << se.getToken().getReportTaskMap() <<  std::endl << std::endl;
//                    std::cout << " - token: " << *ptoken << std::endl;
                    
                    if(headerFlag){
                        
                        headerFlag = false;
                        
                        Writable::strWrite(*imap, ofs, "mapFilename", true);
                        Writable::strWrite(*imap, ofs, "taskFilename", true); 
                        imap->writeHeader(ofs);
                        Writable::sepWrite(*imap, ofs);
                        itasks->writeHeader(ofs);
                        Writable::sepWrite(*imap, ofs);
                        ptoken->writeHeader(ofs);
                        Writable::sepWrite(*imap, ofs);
                        Writable::strWrite(*imap, ofs, "time(s)", true);
                        Writable::endlWrite(*imap, ofs);
                        
                    }

                    Writable::strWrite(*imap, ofs, mapFilename, true);
                    Writable::strWrite(*imap, ofs, taskFilename, true);
                    imap->writeRow(ofs);
                    Writable::sepWrite(*imap, ofs);
                    itasks->writeRow(ofs);
                    Writable::sepWrite(*imap, ofs);
                    ptoken->writeRow(ofs);
                    Writable::sepWrite(*imap, ofs);
                    Writable::strWrite(*imap, ofs, std::to_string(time_span.count()), true);
                    Writable::endlWrite(*imap, ofs);               
                    
                    delete ptoken;
                    
                }
                
                delete itasks;
                
            }
                        
            delete imap;            
                        
        }
        
        ofs.close();        
        
    }
    
    virtual ~MultiSystemExperiment(){}
    
private:
    
    const std::string& resultFilename; 
    const std::vector<unsigned>& seeds;
    const std::vector<std::pair<float, float>>& thresholds;
    const std::vector<std::string>& tokenIds, taskFilenames, mapFilenames;  
    const std::vector<_agent_energy_system>& agent_energy_systems; 
    const std::vector<_agentsTasksAllocator*>& agentsTasksAllocators;
    const unsigned cell_size = 0, timestep = 0, task_threshold = UINT_MAX;

};

#endif /* MULTISYSTEMEXPERIMENT_H */

