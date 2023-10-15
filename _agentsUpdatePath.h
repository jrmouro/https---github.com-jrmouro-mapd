/* 
 * File:   _agentsUpdatePath.h
 * Author: ronaldo
 *
 * Created on 10 de outubro de 2023, 10:32
 */

#ifndef _AGENTSUPDATEPATH_H
#define _AGENTSUPDATEPATH_H

#include <vector>

class _ga_token;
class _stepMap;
class _stepPathAlgorithm;

class _agentsUpdatePath {
    
public:
    
    _agentsUpdatePath(const _stepPathAlgorithm&);

    _agentsUpdatePath(const _agentsUpdatePath&);

    virtual void solve(_ga_token&, _stepMap&, const std::vector<std::pair<int, int>>&) const = 0;   
    
private:
    
    const _stepPathAlgorithm& stepPathAlgorithm;
    
};

#endif /* _AGENTSUPDATEPATH_H */

