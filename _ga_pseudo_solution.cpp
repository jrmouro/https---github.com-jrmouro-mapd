/* 
 * File:   _ga_pseudo_solution.cpp
 * Author: ronaldo
 * 
 * Created on 16 de outubro de 2023, 09:24
 */

#include "_ga_pseudo_solution.h"


_ga_pseudo_solution::_ga_pseudo_solution(){}

_ga_pseudo_solution::_ga_pseudo_solution(const _ga_token& token){
        
    reset(token);

}
    
_ga_pseudo_solution::_ga_pseudo_solution(const _ga_pseudo_solution& other) :
        agents(other.agents), tasks(other.tasks) {}

_ga_pseudo_solution::~_ga_pseudo_solution(){}
      
void _ga_pseudo_solution::swapAgentsByIndexes(unsigned indexAgentA, unsigned indexAgentB){

    if(!agents.empty()){

        unsigned ia = indexAgentA % agents.size();
        unsigned ib = indexAgentB % agents.size();

        const _ga_agent* swapAgent = agents[ia];
        agents[ia] = agents[ib];
        agents[ib] = swapAgent;

    }

}
    
void _ga_pseudo_solution::swapTasksByIndexes(unsigned indexTaskA, unsigned indexTaskB){

    if(!tasks.empty()){

        unsigned ia = indexTaskA % tasks.size();
        unsigned ib = indexTaskB % tasks.size();

        const _task* swapTask = tasks[ia];
        tasks[ia] = tasks[ib];
        tasks[ib] = swapTask;

    }

}
    
void _ga_pseudo_solution::swap(std::set<unsigned> swapAgentsSet, std::set<unsigned> swapTasksSet){
        
    swapAgents(swapAgentsSet);
    swapTasks(swapTasksSet);

}
    
void _ga_pseudo_solution::swapAgents(std::set<unsigned> swapSet){

    std::set<unsigned> swapAux = swapSet;

    for (std::set<unsigned>::iterator ita = swapSet.begin(); ita! = swapSet.end(); ++ita){

        swapAux.erase(swapAux.begin());

        for (std::set<unsigned>::iterator itb = swapAux.begin(); itb! = swapAux.end(); ++itb){

            swapAgentsByIndexes(*ita, *itb);

        }

    }

}

void _ga_pseudo_solution::swapTasks(std::set<unsigned> swapSet){

    std::set<unsigned> swapAux = swapSet;

    for (std::set<unsigned>::iterator ita = swapSet.begin(); ita! = swapSet.end(); ++ita){

        swapAux.erase(swapAux.begin());

        for (std::set<unsigned>::iterator itb = swapAux.begin(); itb! = swapAux.end(); ++itb){

            swapTasksByIndexes(*ita, *itb);

        }

    }

}

void crossover(
    const _ga_pseudo_solution& parentA,
    const _ga_pseudo_solution& parentB, 
    _ga_pseudo_solution& childA, 
    _ga_pseudo_solution& childB){

    if(parentA.isConsistentWith(parentB)){

        agentsCrossover(parentA, parentB, childA, childB);
        tasksCrossover(parentA, parentB, childA, childB);

    } else {

        try {
            std::ostringstream stream;
            stream << "inconsistent pseudo solutions";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }           

}
    
void agentsCrossover(
    const _ga_pseudo_solution& parentA,
    const _ga_pseudo_solution& parentB, 
    _ga_pseudo_solution& childA, 
    _ga_pseudo_solution& childB){

    if(parentA.isConsistentWith(parentB)){

        childA.clear(parentA.agents.size(), parentA.tasks.size());
        childB.clear(parentA.agents.size(), parentA.tasks.size());
        
        std::set<const _ga_agent*> agentsSetA, agentsSetB;
        
        int size = parentA.agents.size()/2;

        for(int i = 0; i < size; i++){
            
            auto paA = parentA.agents[i];
            auto paB = parentB.agents[i];
            
            childA.agents.push_back(paA);
            childB.agents.push_back(paB);
            
            agentsSetA.insert(paA);
            agentsSetB.insert(paB);            

        }   
                
        for(int i = 0; i < parentA.agents.size(); i++){
            
            auto paA = parentA.agents[i];
            auto paB = parentB.agents[i];
            
            auto ita = agentsSetA.find(paB);
            auto itb = agentsSetB.find(paA);
            
            if(ita == agentsSetA.end()) childA.agents.push_back(paB);
            if(itb == agentsSetB.end()) childB.agents.push_back(paA);
            
        }

    } else {

        try {
            std::ostringstream stream;
            stream << "inconsistent pseudo solutions";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }           

}

void tasksCrossover(
    const _ga_pseudo_solution& parentA,
    const _ga_pseudo_solution& parentB, 
    _ga_pseudo_solution& childA, 
    _ga_pseudo_solution& childB){

    if(parentA.isConsistentWith(parentB)){

        childA.clear(parentA.agents.size(), parentA.tasks.size());
        childB.clear(parentA.agents.size(), parentA.tasks.size());
        
        std::set<const _task*> tasksSetA, tasksSetB;
        
        int size = parentA.tasks.size()/2;

        for(int i = 0; i < size; i++){
            
            auto paA = parentA.tasks[i];
            auto paB = parentB.tasks[i];
            
            childA.tasks.push_back(paA);
            childB.tasks.push_back(paB);
            
            tasksSetA.insert(paA);
            tasksSetB.insert(paB);            

        }   
                
        for(int i = 0; i < parentA.tasks.size(); i++){
            
            auto paA = parentA.tasks[i];
            auto paB = parentB.tasks[i];
            
            auto ita = tasksSetA.find(paB);
            auto itb = tasksSetB.find(paA);
            
            if(ita == tasksSetA.end()) childA.tasks.push_back(paB);
            if(itb == tasksSetB.end()) childB.tasks.push_back(paA);
            
        }

    } else {

        try {
            std::ostringstream stream;
            stream << "inconsistent pseudo solutions";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }           

}

virtual void _ga_pseudo_solution::disturb(unsigned agents_size, unsigned task_size, unsigned seed){
    
    std::default_random_engine generator1(seed);
    std::default_random_engine generator2(seed+1);

    std::uniform_int_distribution<unsigned> distribution1(0, agents.size());
    std::uniform_int_distribution<unsigned> distribution2(0, tasks.size());
        
    std::set<unsigned> swapAgentsSet, swapTasksSet;
    
    while(swapAgentsSet.size() < agents_size){
        
        swapAgentsSet.insert(distribution1(generator1));
        
    }
    
    while(swapTasksSet.size() < task_size){
        
        swapTasksSet.insert(distribution2(generator2));
        
    }

    swap(*this, swapAgentsSet, swapTasksSet);
    
}

virtual void _ga_pseudo_solution::disturb(unsigned seed){
    
    std::default_random_engine generator1(seed);
    std::default_random_engine generator2(seed+1);

    std::uniform_int_distribution<unsigned> distribution1(0, agents.size());
    std::uniform_int_distribution<unsigned> distribution2(0, tasks.size());
    
    unsigned number1 = distribution1(generator1);
    unsigned number2 = distribution2(generator2);
    
    std::set<unsigned> swapAgentsSet, swapTasksSet;
    
    for (unsigned i = 0; i < number1; i++) {
        swapAgentsSet.insert(distribution1(generator1));
    }
    
    for (unsigned i = 0; i < number2; i++) {
        swapTasksSet.insert(distribution2(generator2));
    }

    swap(*this, swapAgentsSet, swapTasksSet);
    
}

void _ga_pseudo_solution::listConstAgents(const std::function<bool(const _ga_agent*)>& function) const{
    
    for (auto agent : agents) {
        
        if(function(agent))break;

    }
    
}


void _ga_pseudo_solution::listConstTasks(const std::function<bool(const _task*)>& function) const{
    
    for (auto task : tasks) {
        
        if(function(task))break;

    }
    
}
    
    
void _ga_pseudo_solution::reset(const _ga_token& token){

    clear(token.numberOfAgents(), token.numberOfpendingTasks());

    token.listConstAgents([this](const _ga_agent& agent){

        agents.push_back(&agent);

        return false;

    });

    token.listPendingTasks([this](const _task& task){

        tasks.push_back(&task);

        return false;

    });

}

bool _ga_pseudo_solution::empty() const{
    
    return agents.empty() || tasks.empty();
    
}
    
void _ga_pseudo_solution::clear(unsigned agentsSize, unsigned tasksSize){

    agents.clear();
    tasks.clear();
    
    if(agentsSize > 0){
        agents.resize(agentsSize);
    }
    
    if(tasksSize > 0){
        tasks.resize(tasksSize);
    }

}
    
bool _ga_pseudo_solution::isConsistentWith(const _ga_pseudo_solution& other) const {

    return agents.size() == other.agents.size() && tasks.size() == other.tasks.size();

}