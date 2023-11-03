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
class _agentsTasksAllocator;
class _map;
class _stepMap;
class _allocation;

class _ga_system {
public:
    
    _ga_system(_agentsTasksAllocator&);
                
    _ga_system(const _ga_system&);
    
    virtual ~_ga_system();
    
    virtual void run(const _taskMap& taskMap, _ga_token& token);
    
    virtual bool step(const _taskMap& taskMap, _ga_token& token);
            
private:
    
    _allocation* allocation = nullptr;    
    _agentsTasksAllocator& agentsTasksAllocator;   

};

#endif /* _GA_SYSTEM_H */

