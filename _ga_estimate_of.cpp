/* 
 * File:   _ga_estimate_of.cpp
 * Author: ronaldo
 * 
 * Created on 14 de novembro de 2023, 16:17
 */

#include <chrono>

#include "_ga_estimate_of.h"
#include "_manhattanAlgorithm.h"

const std::map<_ga_solution::EvalType, unsigned>& _ga_estimate_of::evals(const _ga_token& token, _ga_solution& solution) const{
    
    if(!solution.isEvaluated()){
        
//        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    
        
        if(!solution.isAllocated()){
            
            solution.alloc(token);
            
        }
                
        _manhattanAlgorithm manhattanAlgorithm;

        unsigned makespan = 0;
        double energy = .0;
        std::map<const _ga_agent*, std::vector<_stepSite>> agentSites;
        std::map<const _ga_agent*,std::vector<const _task*>> allocation_copy;

        for (auto alloc_pair : solution.allocation_map) {

            allocation_copy.insert(std::pair<const _ga_agent*,std::vector<const _task*>>(alloc_pair.first, alloc_pair.second));

            auto it = agentSites.insert(std::pair<const _ga_agent*, std::vector<_stepSite>>(alloc_pair.first, std::vector<_stepSite>()));

            _stepSite currentSite = it.first->first->currentSite();

            it.first->second.push_back(currentSite);

            bool flag = true;
            for (auto task : alloc_pair.second) {

                unsigned pickupDist = 0, deliveryDist = 0;

                if(true){

                    pickupDist = manhattanAlgorithm.solve(currentSite, task->getPickup());
                    flag = false;

                } else {

                    pickupDist = token.getMap().getEndpointsDistanceAlgorithm().solve(currentSite, task->getPickup());

                }

                deliveryDist = token.getMap().getEndpointsDistanceAlgorithm().solve(task->getPickup(), task->getDelivery());

                currentSite.SetRow(task->getPickup().GetRow());
                currentSite.SetColunm(task->getPickup().GetColunm());
                currentSite.SetStep(currentSite.GetStep() + pickupDist);

                it.first->second.push_back(currentSite);

                currentSite.SetRow(task->getDelivery().GetRow());
                currentSite.SetColunm(task->getDelivery().GetColunm());
                currentSite.SetStep(currentSite.GetStep() + deliveryDist);

                it.first->second.push_back(currentSite);            

            }

        }


        for (auto agentSites_pair1 : agentSites) {
            
            if(agentSites_pair1.second.back().GetStep() > makespan){

                makespan = agentSites_pair1.second.back().GetStep();

            }

            unsigned size = agentSites_pair1.second.size() - 1;

            if(size > 0){

                double area = agentSites_pair1.second[1].siteStepBoxArea(agentSites_pair1.second[0]);

                double energy_cost = (agentSites_pair1.second[1].GetStep() - agentSites_pair1.second[0].GetStep()) *
                                    token.getAgent_energy_system().getMovingRegime();

                if(area != 0){

                    for (auto agentSites_pair2 : agentSites) {

                        if(agentSites_pair1.first != agentSites_pair2.first){

                            for (auto stepSite : agentSites_pair2.second) {

                                if(stepSite.insideStepSiteBox(agentSites_pair1.second[0], agentSites_pair1.second[1])){

                                    energy += energy_cost / area;

                                }

                            }

                        }

                    }      

                }

            }

            for (int i = 1; i < size; i++) {

                double area = agentSites_pair1.second[i+1].siteStepBoxArea(agentSites_pair1.second[i]);

                double energy_cost = (agentSites_pair1.second[i + 1].GetStep() - agentSites_pair1.second[i].GetStep()) *
                                    token.getAgent_energy_system().getMovingRegime();

                if(area != 0){

                    for (auto agentSites_pair2 : agentSites) {

                        if(agentSites_pair1.first != agentSites_pair2.first){

                            for (auto stepSite : agentSites_pair2.second) {

                                if(stepSite.insideStepSiteBox(agentSites_pair1.second[i], agentSites_pair1.second[i+1])){

                                    energy += energy_cost / area;

                                }

                            }

                        }

                    }  

                }

            }

        }

        energy *= token.getStepMap().getStep_size();

        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan));
        solution.evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy));

//        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
//        
//        std::cout << "_ga_estimate_of time span: " << time_span.count() << std::endl;
        
    }
    
    return solution.evals;
    
}

