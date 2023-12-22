/* 
 * File:   _stepPathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:35
 */

#ifndef _STEPPATHALGORITHM_H
#define _STEPPATHALGORITHM_H

#include "_site.h"
//#include "_stepMap.h"
#include "_env_map.h"
#include "_stepPath.h"
#include "_agent.h"

class _token;
class _stepPathAlgorithm {
public:
    
//    virtual bool solve(const _stepMap& map, _stepPath& path, const _site& goal, int type) const = 0;
    
    virtual bool solve(const _env_map& map, _stepPath& path, const _site& goal, int type) const = 0;
    
    bool solve(const _token& token, const _agent& agent, _stepPath& path, const _site& goal) const ;
    
};

#endif /* _STEPPATHALGORITHM_H */

