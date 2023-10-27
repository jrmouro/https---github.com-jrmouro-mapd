/* 
 * File:   _ga_system.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 13:44
 */

#include "_ga_system.h"
#include "_ga_token.h"
#include "_stepMap.h"
#include "_taskMap.h"
#include "_agentsTasksAllocator.h"
#include "_agentsUpdatePath.h"
#include "_allocation.h"
#include "_ga_solution.h"

_ga_system::_ga_system(const _agentsTasksAllocator& agentsTasksAllocator) :
                agentsTasksAllocator(agentsTasksAllocator){}
                
_ga_system::_ga_system(const _ga_system& other) :  
            agentsTasksAllocator(other.agentsTasksAllocator){
    if(other.allocation != nullptr)
        allocation = other.allocation->clone();
}

_ga_system::~_ga_system(){
                
    if(allocation != nullptr) delete (_ga_solution*)allocation;
    
}

bool _ga_system::step(const _taskMap& taskMap, _ga_token& token){ 
    
    if(token.getCurrentStep() < token.getStepMap().getStep_size() && !(taskMap.getLastTask() < token.getCurrentStep() && token.isIdle())){
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getCurrentStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        });                
        
        if(allocation == nullptr){

            allocation = agentsTasksAllocator.solve(token);

        } else {

            if(addNewTask || !allocation->isValid()){

                delete allocation;
              
                allocation = agentsTasksAllocator.solve(token);

           } 

        }
        
        allocation->nextPlanningUpdate(token, [&token](const _ga_agent* agent, const _task* task){
            
            if(task != nullptr){
            
                return token.updateAgentTaskPath(agent->id(), task->id());
            
            } else {
                
                return token.updateAgentTaskPath(agent->id(), INT_MIN);
                
            }
            
        });
        
        token.stepping();       
        
        return true;
        
    }
          
    return false;

}

void _ga_system::run(const _taskMap& taskMap, _ga_token& token){    
        
    while(token.getCurrentStep() < token.getStepMap().getStep_size() && !(taskMap.getLastTask() < token.getCurrentStep() && token.isIdle())){
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getCurrentStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        });
        
        if(allocation == nullptr){

            allocation = agentsTasksAllocator.solve(token);

        } else {

            if(addNewTask || !allocation->isValid()){

                delete allocation;

                allocation = agentsTasksAllocator.solve(token);

           } 

        }
        
        allocation->nextPlanningUpdate(token, [&token](const _ga_agent* agent, const _task* task){
            
            if(task != nullptr){
            
                return token.updateAgentTaskPath(agent->id(), task->id());
            
            } else {
                
                return token.updateAgentTaskPath(agent->id(), INT_MIN);
                
            }
            
        });
        
        token.stepping();
        
    }
    
}