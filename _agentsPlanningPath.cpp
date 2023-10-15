/* 
 * File:   _agentsPlanningPath.cpp
 * Author: ronaldo
 * 
 * Created on 11 de outubro de 2023, 14:35
 */

#include "_agentsPlanningPath.h"
#include <climits>
#include <unordered_set>
#include <vector>
#include <unordered_map>

void _agentsPlanningPath::solve(
        const _ga_token& token, 
        const _map& map, 
        const _stepMap& stepMap,
        std::unordered_map<int, std::vector<int>>& agentsTasksAllocation, 
        std::vector<std::pair<int, int>>& planningPath) const{
    
    for(auto alloc = agentsTasksAllocation.begin(); alloc != agentsTasksAllocation.end(); alloc++){
        
        int taskId = INT_MIN;
        if(!alloc->second.empty()) {
            taskId = alloc->second[0];
            alloc->second.erase(alloc->second.begin());
        }
        
        planningPath.push_back(std::pair<int, int>(alloc->first, taskId));
        
    }
    
}