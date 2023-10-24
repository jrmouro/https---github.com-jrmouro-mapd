/* 
 * File:   _ga_solution.cpp
 * Author: ronaldo
 * 
 * Created on 17 de outubro de 2023, 09:55
 */

#include "_ga_solution.h"
#include "_endpointsDistanceAlgorithm.h"



_ga_solution::_ga_solution(){}

_ga_solution::_ga_solution(const _ga_token& token):_ga_pseudo_solution(token){}

_ga_solution::_ga_solution(const _ga_solution& other) :
        _ga_pseudo_solution(other),
        allocation(other.allocation), 
        evals(other.evals) {}
    
_ga_solution::~_ga_solution(){}

unsigned _ga_solution::evaluate(const _ga_token& token, const _ga_solution::EvalType& evalType){
    
    auto evalMap = evaluate(token);
    
    std::map<EvalType, unsigned>::const_iterator evalTypeIt = evals.find(evalType);
    
    if(evalTypeIt == evals.end()){
        
        try {
            std::ostringstream stream;
            stream << "eval type not found: " << evalType;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    return evalTypeIt->second;
    
}
    
const std::map<_ga_solution::EvalType, unsigned>& _ga_solution::evaluate(const _ga_token& token){

    if(evals.empty()){
        
        alloc(token);

    }

    return evals;

}

void _ga_solution::disturb(unsigned seed){
    evals.clear();
    allocation.clear();
    _ga_pseudo_solution::disturb(seed);
}

void _ga_solution::disturb(unsigned agents_size, unsigned task_size, unsigned seed){
    evals.clear();
    allocation.clear();
    _ga_pseudo_solution::disturb(agents_size, task_size, seed);
}

_ga_solution* _ga_solution::get_disturb(unsigned seed) const{
        
    auto ret = new _ga_solution(*this);
    ret.disturb(seed);
    return ret;

}

_ga_solution* _ga_solution::get_disturb(unsigned agents_size, unsigned task_size, unsigned seed) const{

    auto ret = new _ga_solution(*this);
    ret.disturb(agents_size, task_size, seed);
    return ret;

}

std::pair<_ga_solution*,_ga_solution*> _ga_solution::get_crossover(const _ga_solution& other) const{
        
    _ga_solution* childA = new _ga_solution();
    _ga_solution* childB = new _ga_solution();

    crossover(*this, other, *childA, *childB);

    return std::pair<_ga_solution*,_ga_solution*>(childA, childB);

}

bool _ga_solution::dominate(const _ga_token& token, const _ga_solution& other)const{
    
    auto other_eval = other.evaluate(token);
    auto this_eval = evaluate(token);
    
    bool flag = false;
    
    for (auto tev : this_eval) {
        
        std::map<_ga_solution::EvalType, unsigned>::const_iterator it = other_eval.find(tev.first);
                
        if(it->second < tev.second){
            
            return false;
            
        } 
        
        
        if(tev.second < it->second){
            
            flag = true;
            
        }
        

    }
    
    return flag;
    
}

_ga_solution* _ga_solution::randon(unsigned seed) const{
    
    auto ret = new _ga_solution(*this);
    
    ret->disturb(seed);
    
    return ret;
    
}

void _ga_solution::planning(const _ga_token& token, const std::function<bool(const _ga_agent* agent, const _task* task)>& function){
    
    if(evals.empty()){
        
        alloc(token);

    }
    
    for (const _ga_agent* agent : agents) {
        
        std::map<const _ga_agent*,std::vector<const _task*>>::iterator agent_alloc_it = allocation.find(agent);
        
        if(agent_alloc_it != allocation.end()){
            
            if(agent_alloc_it->second.empty()){
                
                function(agent, nullptr);
                
            } else {
                
                if(function(agent, agent_alloc_it->second.front())){
                    
                    agent_alloc_it->second.erase(agent_alloc_it->second.begin());
                    
                }
                
            }
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "allocation error - agent not found: " << *agent;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
}

void _ga_solution::alloc(const _ga_token& token){
        
    std::map<const _ga_agent*, _stepSite> agentGoalSites;
    const _ga_agent* agent_min = nullptr;
    _stepSite stepSite_min;
    unsigned makespan = 0, energy = 0;
    
    allocation.clear();
    evals.clear();

    for (const _ga_agent* agent : agents) {

        _stepSite goal = agent->getGoalSite();
        
        if(makespan < goal.GetStep()){
            
            makespan = goal.GetStep();
            
        }

        if(agent_min == nullptr){

            agent_min = agent;
            stepSite_min = goal;

        }             

        if(stepSite_min.GetStep() < goal.GetStep()){

            agent_min = agent;
            stepSite_min = goal;

        }

        agentGoalSites.insert(std::pair<const _ga_agent*, _stepSite>(agent, goal));
        
        allocation.insert(std::pair<const _ga_agent*,std::vector<const _task*>>(agent, std::vector<const _task*>()));

    }


    if(agent_min != nullptr){

        for (const _task* task : tasks) {

            unsigned dist = token.getMap().getEndpointsDistanceAlgorithm().solve(stepSite_min, task->getPickup());
            energy += dist * token.getAgent_energy_system().getMovingRegime();
           
            dist = dist + token.getMap().getEndpointsDistanceAlgorithm().solve(task->getPickup(), task->getDelivery());
            
            std::map<const _ga_agent*, _stepSite>::iterator agentsGoalSitesIterator = agentGoalSites.find(agent_min);

            if(agentsGoalSitesIterator != agentGoalSites.end()){

                agentsGoalSitesIterator->second.SetRow(task->getDelivery().GetRow());
                agentsGoalSitesIterator->second.SetColunm(task->getDelivery().GetColunm());
                
                unsigned goalMakespan = agentsGoalSitesIterator->second.GetStep() + dist;
                agentsGoalSitesIterator->second.SetStep(goalMakespan);
                
                if(makespan < goalMakespan){
            
                    makespan = goalMakespan;

                }
                
                std::map<const _ga_agent*,std::vector<const _task*>>::iterator alloc_it = allocation.find(agent_min);

                if(alloc_it != allocation.end()){

                    alloc_it->second.push_back(task);

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "agent not found: " << *agent_min;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

                for (const auto& goal : agentGoalSites) {

                    if(stepSite_min.GetStep() < goal.second.GetStep()){

                        agent_min = goal.first;
                        stepSite_min = goal.second;

                    }

                }                    
                    
            }else{

                try {
                    std::ostringstream stream;
                    stream << "agent not found" << *agent_min;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        }

    }
    
    evals.insert(std::pair<EvalType, unsigned>(EvalType::makespan, makespan));
    evals.insert(std::pair<EvalType, unsigned>(EvalType::energy, energy));
    
}

