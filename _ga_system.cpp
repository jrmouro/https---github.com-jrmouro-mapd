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
#include "_agentsPlanningPath.h"

_ga_system::_ga_system(
            const _agentsPlanningPath& agentsPlanningPath, 
            const _agentsTasksAllocator& agentsTasksAllocator, 
            const _agentsUpdatePath& agentsUpdatePath) :
                agentsPlanningPath(agentsPlanningPath), 
                agentsTasksAllocator(agentsTasksAllocator), 
                agentsUpdatePath(agentsUpdatePath) {}
                
_ga_system::_ga_system(const _ga_system& other) :
            agentsTasksAllocation(other.agentsTasksAllocation), 
            agentsPlanningPath(other.agentsPlanningPath), 
            agentsTasksAllocator(other.agentsTasksAllocator), 
            agentsUpdatePath(other.agentsUpdatePath) {}

bool _ga_system::step(const _taskMap& taskMap, _ga_token& token){ 
    
    if(token.getCurrentStep() < token.getStepMap().getStep_size() || (taskMap.getLastTask() < token.getCurrentStep() && token.isIdle())){
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getCurrentStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        });
                
        
        if(addNewTask){
            
            agentsTasksAllocation.clear();
            
            agentsTasksAllocator.solve(token, agentsTasksAllocation);
            
        }
        
        std::vector<std::pair<int, int>> planning;
        
        agentsPlanningPath.solve(token, agentsTasksAllocation, planning);
        
        agentsUpdatePath.solve(token, planning);
        
        token.stepping();       
        
        return true;
        
    }
          
    return false;

}

void _ga_system::run(const _taskMap& taskMap, _ga_token& token){
    
    while(token.getCurrentStep() < token.getStepMap().getStep_size() || (taskMap.getLastTask() < token.getCurrentStep() && token.isIdle())){
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getCurrentStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        });
                        
        if(addNewTask){
            
            agentsTasksAllocation.clear();
            
            agentsTasksAllocator.solve(token, agentsTasksAllocation);
            
        }
        
        std::vector<std::pair<int, int>> planning;
        
        agentsPlanningPath.solve(token, agentsTasksAllocation, planning);
        
        agentsUpdatePath.solve(token, planning);
        
        token.stepping();
        
    }
    
}