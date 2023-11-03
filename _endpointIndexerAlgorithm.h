/* 
 * File:   _endpointIndexerAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 17:07
 */

#ifndef _ENDPOINTINDEXERALGORITHM_H
#define _ENDPOINTINDEXERALGORITHM_H

#include <vector>
#include "_site.h"
#include "_agent.h"
#include "_ga_agent.h"
#include "_task.h"

class _token;
class _ga_token;
class _endpointIndexerAlgorithm {
public:
    
    virtual _endpointIndexerAlgorithm* getInstance() const = 0;
    
    virtual void solve(
            const _token& token, 
            const _site& site, 
            const _site& reference, 
            std::vector<_site>& siteIndex) const = 0;
    
    void solve(            
            const _token& token,
            const _site& site, 
            const _agent& agent, 
            std::vector<_site>& siteIndex) const;
    
    virtual void ga_solve_to_site(
            const _ga_token& token, 
            const _site& site, 
            const _site& reference, 
            std::vector<_site>& siteIndex) const = 0;
    
    void ga_solve_to_agent(            
            const _ga_token& token,
            const _site& site, 
            const _ga_agent& agent, 
            std::vector<_site>& siteIndex) const;

};

#endif /* _ENDPOINTINDEXERALGORITHM_H */

