
/* 
 * File:   _agentsPlanningPath.h
 * Author: ronaldo
 *
 * Created on 11 de outubro de 2023, 14:35
 */

#ifndef _AGENTSPLANNINGPATH_H
#define _AGENTSPLANNINGPATH_H

#include <unordered_map>
#include <vector>

class _ga_token;
class _map;
class _stepMap;

class _agentsPlanningPath {
    
public:
    
    virtual void solve(
        const _ga_token&, 
        const _map&, 
        const _stepMap&,
        std::unordered_map<int, std::vector<int>>&, 
        std::vector<std::pair<int, int>>&) const = 0;

};

#endif /* _AGENTSPLANNINGPATH_H */

