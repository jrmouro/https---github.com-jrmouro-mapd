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
            const _agentsUpdatePath& agentsUpdatePath, 
            const _map& map, 
            const _stepMap& stepMap) :
                agentsPlanningPath(agentsPlanningPath), 
                agentsTasksAllocator(agentsTasksAllocator), 
                agentsUpdatePath(agentsUpdatePath), 
                map(map), 
                stepMap(stepMap) {}
                
_ga_system::_ga_system(const _ga_system& other) :
            agentsTasksAllocation(other.agentsTasksAllocation), 
            agentsPlanningPath(other.agentsPlanningPath), 
            agentsTasksAllocator(other.agentsTasksAllocator), 
            agentsUpdatePath(other.agentsUpdatePath), 
            map(other.map), 
            stepMap(other.stepMap) {}

const _map& _ga_system::getMap() const {
    return map;
}

bool _ga_system::step(const _taskMap& taskMap, _ga_token& token){ 
    
    if(token.getStep() < stepMap.getStep_size() || (taskMap.getLastTask() < token.getStep() && token.isIdle())){
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        });
                
        
        if(addNewTask){
            
            agentsTasksAllocation.clear();
            
            agentsTasksAllocator.solve(token, map, stepMap, agentsTasksAllocation);
            
        }
        
        std::vector<std::pair<int, int>> planning;
        
        agentsPlanningPath.solve(token, map, stepMap, agentsTasksAllocation, planning);
        
        agentsUpdatePath.solve(token, stepMap, planning);
        
        token.stepping(map);       
        
        return true;
        
    }
          
    return false;

}

void _ga_system::run(const _taskMap& taskMap, _ga_token& token){
    
    while(token.getStep() < stepMap.getStep_size() || (taskMap.getLastTask() < token.getStep() && token.isIdle())){
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        });
                        
        if(addNewTask){
            
            agentsTasksAllocation.clear();
            
            agentsTasksAllocator.solve(token, map, stepMap, agentsTasksAllocation);
            
        }
        
        std::vector<std::pair<int, int>> planning;
        
        agentsPlanningPath.solve(token, map, stepMap, agentsTasksAllocation, planning);
        
        agentsUpdatePath.solve(token, stepMap, planning);
        
        token.stepping(map);
        
    }
    
}