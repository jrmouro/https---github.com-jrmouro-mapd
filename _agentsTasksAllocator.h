/* 
 * File:   _agentsTasksAllocator.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 13:27
 */

#ifndef _AGENTSTASKSALLOCATOR_H
#define _AGENTSTASKSALLOCATOR_H

#include <vector>
#include <map>

class _agentsTasksAllocator {
public:
    virtual void solve(const _ga_token&, const _map&, const _stepMap&, std::unordered_map<int, std::vector<int>>&) const = 0;
};

#endif /* _AGENTSTASKSALLOCATOR_H */

