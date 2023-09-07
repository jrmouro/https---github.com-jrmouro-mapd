/* 
 * File:   _pathToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 09:11
 */

#ifndef _PATHTOAGENTALGORITHM_H
#define _PATHTOAGENTALGORITHM_H

#include "_stepPathAlgorithm.h"
#include "_agent.h"
#include "_stepPath.h"
#include "_site.h"
#include "_stepMap.h"
#include "_token.h"

class _pathToAgentAlgorithm {
public:
    
    _pathToAgentAlgorithm(const _stepPathAlgorithm& stepPathAlgorithm) :
    stepPathAlgorithm(stepPathAlgorithm) { }
    
    _pathToAgentAlgorithm(const _pathToAgentAlgorithm& other) :
    stepPathAlgorithm(other.stepPathAlgorithm) { }
    
    virtual bool solve(
        const _token& token, 
        const _agent& agent, 
        const _site& goalSite, 
        _stepPath& path) const {
        
        bool flag = stepPathAlgorithm.solve(token, agent, path, goalSite);
        
        if(flag){
                        
            return true;            
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "unsolved goal site path: " << goalSite;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
        return false;
        
    }
    
private:
    
    const _stepPathAlgorithm& stepPathAlgorithm;
    
};

#endif /* _PATHTOAGENTALGORITHM_H */

