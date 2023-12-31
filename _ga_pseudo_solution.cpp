/* 
 * File:   _ga_pseudo_solution.cpp
 * Author: ronaldo
 * 
 * Created on 16 de outubro de 2023, 09:24
 */

#include "_ga_pseudo_solution.h"
#include <random>


_ga_pseudo_solution::_ga_pseudo_solution(){}

_ga_pseudo_solution::_ga_pseudo_solution(const _ga_token& token){
        
    reset(token);

}
    
_ga_pseudo_solution::_ga_pseudo_solution(const _ga_pseudo_solution& other) :
        agents(other.agents), tasks(other.tasks) {}

_ga_pseudo_solution::~_ga_pseudo_solution(){}

unsigned _ga_pseudo_solution::agentsSize()const{
    return agents.size();
}

unsigned _ga_pseudo_solution::tasksSize()const{
    return tasks.size();
}
      
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
    
void _ga_pseudo_solution::swap(const std::set<unsigned>& swapAgentsSet, const std::set<unsigned>& swapTasksSet){
        
    swapAgents(swapAgentsSet);
    swapTasks(swapTasksSet);

}
    
void _ga_pseudo_solution::swapAgents(const std::set<unsigned>& swapSet){

    std::set<unsigned> swapAux = swapSet;

    for (std::set<unsigned>::iterator ita = swapSet.begin(); ita != swapSet.end(); ++ita){

        swapAux.erase(swapAux.begin());

        for (std::set<unsigned>::iterator itb = swapAux.begin(); itb != swapAux.end(); ++itb){

            swapAgentsByIndexes(*ita, *itb);

        }

    }

}

void _ga_pseudo_solution::swapTasks(const std::set<unsigned>& swapSet){

    std::set<unsigned> swapAux = swapSet;

    for (std::set<unsigned>::iterator ita = swapSet.begin(); ita != swapSet.end(); ++ita){

        swapAux.erase(swapAux.begin());

        for (std::set<unsigned>::iterator itb = swapAux.begin(); itb != swapAux.end(); ++itb){

            swapTasksByIndexes(*ita, *itb);

        }

    }

}

_ga_pseudo_solution& _ga_pseudo_solution::operator=(const _ga_pseudo_solution& right) {
        
    if (this == &right)
        return *this;

    agents = right.agents;
    tasks = right.tasks;        

    return *this;
}
    
std::ostream& operator<<(std::ostream& os, const _ga_pseudo_solution& obj) {
    
    os << "agents (number): " << obj.agents.size() << std::endl;
    
    for (auto agent : obj.agents) {
        os << agent->id() << ", ";
    }
    
    os << std::endl << "tasks (number): " << obj.tasks.size() << std::endl;
    for (auto task : obj.tasks) {
        
        if(task != nullptr){
            os << task->id() << ", ";        
        } else {
            os << "#, ";   
        }
    }
    
    os << std::endl;
        
    return os;
}

void crossover(
    const _ga_pseudo_solution& parentA,
    const _ga_pseudo_solution& parentB, 
    _ga_pseudo_solution& childA, 
    _ga_pseudo_solution& childB,
    _ga_pseudo_solution& childC, 
    _ga_pseudo_solution& childD,
    unsigned agentsCrossoverPoint,
    unsigned tasksCrossoverPoint){

    if(parentA.isConsistentWith(parentB)){

        agentsCrossover(parentA, parentB, childA, childB, childC, childD, agentsCrossoverPoint);
        tasksCrossover(parentA, parentB, childA, childB, childC, childD, tasksCrossoverPoint);

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
    _ga_pseudo_solution& childB,
    _ga_pseudo_solution& childC, 
    _ga_pseudo_solution& childD,
    unsigned crossoverPoint){
    
    if(!parentA.agents.empty()){

        if(parentA.isConsistentWith(parentB)){

            childA.clearResize(parentA.agents.size(), 0);
            childB.clearResize(parentA.agents.size(), 0);
            childC.clearResize(parentA.agents.size(), 0);
            childD.clearResize(parentA.agents.size(), 0);

            std::set<const _ga_agent*> agentsSetA, agentsSetB, agentsSetC, agentsSetD;

            int size = crossoverPoint % parentA.agents.size();

            int i = 0;
            for(; i < size; i++){

                auto paA = parentA.agents[i];
                auto paB = parentB.agents[i];

                childA.agents[i] = paA;
                childB.agents[i] = paB;

                agentsSetA.insert(paA);
                agentsSetB.insert(paB);            

            }   
            
            int f = size;
            int ii = 0;
            for(; f < parentA.agents.size(); f++){

                auto paA = parentA.agents[f];
                auto paB = parentB.agents[f];

                childC.agents[ii] = paA;
                childD.agents[ii] = paB;

                agentsSetC.insert(paA);
                agentsSetD.insert(paB);  
                
                ii++;

            } 

            int l = i;
            for(int j = 0; j < parentA.agents.size(); j++){

                auto paA = parentA.agents[j];
                auto paB = parentB.agents[j];

                auto ita = agentsSetA.find(paB);
                auto itb = agentsSetB.find(paA);

                if(ita == agentsSetA.end()) childA.agents[l++] = paB;
                if(itb == agentsSetB.end()) childB.agents[i++] = paA;

            }
            
            int h = ii;            
            for(int j = 0; j < parentA.agents.size(); j++){

                auto paA = parentA.agents[j];
                auto paB = parentB.agents[j];

                auto ita = agentsSetC.find(paB);
                auto itb = agentsSetD.find(paA);

                if(ita == agentsSetC.end()) childC.agents[ii++] = paB;
                if(itb == agentsSetD.end()) childD.agents[h++] = paA;

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

}

void tasksCrossover(
    const _ga_pseudo_solution& parentA,
    const _ga_pseudo_solution& parentB, 
    _ga_pseudo_solution& childA, 
    _ga_pseudo_solution& childB,
    _ga_pseudo_solution& childC, 
    _ga_pseudo_solution& childD,
    unsigned crossoverPoint){
    
    if(!parentA.tasks.empty()){

        if(parentA.isConsistentWith(parentB)){

            childA.clearResize(0, parentA.tasks.size());
            childB.clearResize(0, parentA.tasks.size());
            childC.clearResize(0, parentA.tasks.size());
            childD.clearResize(0, parentA.tasks.size());

            std::set<const _task*> tasksSetA, tasksSetB, tasksSetC, tasksSetD;

            int size = crossoverPoint % parentA.tasks.size();

            int i = 0;
            for(; i < size; i++){

                auto paA = parentA.tasks[i];
                auto paB = parentB.tasks[i];

                childA.tasks[i] = paA;
                childB.tasks[i] = paB;

                tasksSetA.insert(paA);
                tasksSetB.insert(paB);            

            } 
            
            int f = size;
            int ii = 0;
            for(; f < parentA.tasks.size(); f++){

                auto paA = parentA.tasks[f];
                auto paB = parentB.tasks[f];

                childC.tasks[ii] = paA;
                childD.tasks[ii] = paB;

                tasksSetC.insert(paA);
                tasksSetD.insert(paB);  
                
                ii++;

            } 

            int l = i;
            for(int j = 0; j < parentA.tasks.size(); j++){

                auto paA = parentA.tasks[j];
                auto paB = parentB.tasks[j];

                auto ita = tasksSetA.find(paB);
                auto itb = tasksSetB.find(paA);

                if(ita == tasksSetA.end()) childA.tasks[i++] = paB;
                if(itb == tasksSetB.end()) childB.tasks[l++] = paA;


            }
            
            int h = ii;            
            for(int j = 0; j < parentA.tasks.size(); j++){

                auto paA = parentA.tasks[j];
                auto paB = parentB.tasks[j];

                auto ita = tasksSetC.find(paB);
                auto itb = tasksSetD.find(paA);

                if(ita == tasksSetC.end()) childC.tasks[ii++] = paB;
                if(itb == tasksSetD.end()) childD.tasks[h++] = paA;

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

}

void _ga_pseudo_solution::disturb(unsigned agents_size, unsigned task_size, std::default_random_engine& generator){
    
    
    std::uniform_int_distribution<unsigned> distribution1(0, agents.size());
    std::uniform_int_distribution<unsigned> distribution2(0, tasks.size());
        
    std::set<unsigned> swapAgentsSet, swapTasksSet;
    
    while(swapAgentsSet.size() < agents_size){
        
        swapAgentsSet.insert(distribution1(generator));
        
    }
    
    while(swapTasksSet.size() < task_size){
        
        swapTasksSet.insert(distribution2(generator));
        
    }

    swap(swapAgentsSet, swapTasksSet);
    
}

void _ga_pseudo_solution::disturb(std::default_random_engine& generator){
    
    std::uniform_int_distribution<unsigned> distribution1(0, agents.size());
    std::uniform_int_distribution<unsigned> distribution2(0, tasks.size());
    
    unsigned number1 = distribution1(generator);
    unsigned number2 = distribution2(generator);
    
    std::set<unsigned> swapAgentsSet, swapTasksSet;
    
    for (unsigned i = 0; i < number1; i++) {
        swapAgentsSet.insert(distribution1(generator));
    }
    
    for (unsigned i = 0; i < number2; i++) {
        swapTasksSet.insert(distribution2(generator));
    }

    swap(swapAgentsSet, swapTasksSet);
    
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

    clearResize(token.numberOfAgents(), token.numberOfpendingTasks());

    token.listConstAgents([this](unsigned index, const _ga_agent& agent){

        agents[index] = &agent;

        return false;

    });

    token.listPendingTasks([this](unsigned index, const _task& task){

        tasks[index] = &task;

        return false;

    });

}

bool _ga_pseudo_solution::empty() const{
    
    return agents.empty() || tasks.empty();
    
}
    
void _ga_pseudo_solution::clearResize(unsigned agentsSize, unsigned tasksSize){
    
    if(agentsSize > 0){
        agents.clear();
        agents.resize(agentsSize);
    }
    
    if(tasksSize > 0){
        tasks.clear();
        tasks.resize(tasksSize);
    }
    
}
    
bool _ga_pseudo_solution::isConsistentWith(const _ga_pseudo_solution& other) const {

    return agents.size() == other.agents.size() && tasks.size() == other.tasks.size();

}