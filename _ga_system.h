/* 
 * File:   _ga_system.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 13:44
 */

#ifndef _GA_SYSTEM_H
#define _GA_SYSTEM_H

#include <vector>
#include <unordered_map>

#include "_stepMap.h"

class _taskMap;
class _ga_token;
class _agentsPlanningPath;
class _agentsTasksAllocator;
class _agentsUpdatePath;
class _map;
class _stepMap;

class _ga_system {
public:
    
    _ga_system(
            const _agentsPlanningPath&, 
            const _agentsTasksAllocator&, 
            const _agentsUpdatePath&);
                
    _ga_system(const _ga_system&);
    
    virtual void run(const _taskMap& taskMap, _ga_token& token);
    
    virtual bool step(const _taskMap& taskMap, _ga_token& token);
            
private:
    std::unordered_map<int, std::vector<int>> agentsTasksAllocation;
    const _agentsPlanningPath& agentsPlanningPath;
    const _agentsTasksAllocator& agentsTasksAllocator;
    const _agentsUpdatePath& agentsUpdatePath;

};

#endif /* _GA_SYSTEM_H */

