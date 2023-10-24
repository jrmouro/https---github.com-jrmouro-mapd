/* 
 * File:   _greedy_agentsTasksAllocator.cpp
 * Author: ronaldo
 * 
 * Created on 14 de outubro de 2023, 16:22
 */

#include "_greedy_agentsTasksAllocator.h"

void _greedy_agentsTasksAllocator::solve(
        const std::vector<const _ga_agent*>& agents, 
        const std::vector<const _task*>& tasks, 
        const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm, 
        std::unordered_map<const _ga_agent*, std::vector<const _task*>>& allocation) const{
    
    std::map<const _ga_agent*, _stepSite> agentsGoalSites;
    const _ga_agent* agent_min = nullptr;
    _stepSite stepSite_min;
    
    for (auto agent : agents) {
        
        _stepSite goal = agent->getGoalSite();

        if(agent_min == nullptr){

            agent_min = agent;
            stepSite_min = goal;

        }             

        if(stepSite_min.GetStep() < goal.GetStep()){

            agent_min = agent;
            stepSite_min = goal;

        }

        agentsGoalSites.insert(std::pair<const _ga_agent*, _stepSite>(agent, goal));

    }


    if(agent_min != nullptr){
        
        for (auto task : tasks) {
            
            unsigned dist = endpointsDistanceAlgorithm.solve(stepSite_min, task->getPickup());
            dist = dist + endpointsDistanceAlgorithm.solve(task->getPickup(), task->getDelivery());

            std::map<const _ga_agent*, _stepSite>::iterator agentsGoalSitesIterator = agentsGoalSites.find(agent_min);

            if(agentsGoalSitesIterator != agentsGoalSites.end()){

                agentsGoalSitesIterator->second.SetRow(task->getDelivery().GetRow());
                agentsGoalSitesIterator->second.SetColunm(task->getDelivery().GetColunm());
                agentsGoalSitesIterator->second.SetStep(agentsGoalSitesIterator->second.GetStep() + dist);

                std::unordered_map<const _ga_agent*, std::vector<const _task*>>::iterator alloc_it = allocation.find(agent_min);

                if(alloc_it == allocation.end()){

                    auto insert_alloc = allocation.insert(std::pair<const _ga_agent*, std::vector<const _task*>>(agent_min, std::vector<const _task*>()));

                    alloc_it = insert_alloc.first;

                } 

                alloc_it->second.push_back(task);

                for (const auto& goal : agentsGoalSites) {

                    if(stepSite_min.GetStep() < goal.second.GetStep()){

                        agent_min = goal.first;
                        stepSite_min = goal.second;

                    }

                }

            }else{

                try {
                    std::ostringstream stream;
                    stream << "agent id not found" << agent_min->id();
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        }

    }
    
}

void _greedy_agentsTasksAllocator::solve(
        const _ga_token& token, 
        const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm,
        std::unordered_map<int, std::vector<int>>& allocation) const {
        
    std::map<const _ga_agent*, _stepSite> agentsGoalSites;
    const _ga_agent* agent_min = nullptr;
    _stepSite stepSite_min;

    token.listConstAgents([&agentsGoalSites, &agent_min, &stepSite_min](const _ga_agent& agent){

        _stepSite goal = agent.getGoalSite();

        if(agent_min == nullptr){

            agent_min = &agent;
            stepSite_min = goal;

        }             

        if(stepSite_min.GetStep() < goal.GetStep()){

            agent_min = &agent;
            stepSite_min = goal;

        }

        agentsGoalSites.insert(std::pair<const _ga_agent*, _stepSite>(&agent, goal));

        return false;

    });

    if(agent_min != nullptr){

        token.listPendingTasks([&agent_min, &stepSite_min, &agentsGoalSites, &allocation, token, endpointsDistanceAlgorithm](const _task& task){

            unsigned dist = endpointsDistanceAlgorithm.solve(stepSite_min, task.getPickup());
            dist = dist + endpointsDistanceAlgorithm.solve(task.getPickup(), task.getDelivery());

            std::map<const _ga_agent*, _stepSite>::iterator agentsGoalSitesIterator = agentsGoalSites.find(agent_min);

            if(agentsGoalSitesIterator != agentsGoalSites.end()){

                agentsGoalSitesIterator->second.SetRow(task.getDelivery().GetRow());
                agentsGoalSitesIterator->second.SetColunm(task.getDelivery().GetColunm());
                agentsGoalSitesIterator->second.SetStep(agentsGoalSitesIterator->second.GetStep() + dist);

                std::unordered_map<int, std::vector<int>>::iterator alloc_it = allocation.find(agent_min->id());

                if(alloc_it == allocation.end()){

                    auto insert_alloc = allocation.insert(std::pair<int, std::vector<int>>(agent_min->id(), std::vector<int>()));

                    alloc_it = insert_alloc.first;

                } 

                alloc_it->second.push_back(task.id());

                for (const auto& goal : agentsGoalSites) {

                    if(stepSite_min.GetStep() < goal.second.GetStep()){

                        agent_min = goal.first;
                        stepSite_min = goal.second;

                    }

                }

            }else{

                try {
                    std::ostringstream stream;
                    stream << "agent id not found" << agent_min->id();
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

            return false;

        });

    }

}