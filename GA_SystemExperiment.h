/* 
 * File:   GA_SystemExperiment.h
 * Author: ronaldo
 *
 * Created on 12 de outubro de 2023, 09:24
 */

#ifndef GA_SYSTEMEXPERIMENT_H
#define GA_SYSTEMEXPERIMENT_H

#include <string>

#include "Experiment.h"
#include "_ga_system.h"
#include "_ga_token.h"
#include "_taskMap.h"

class GA_SystemExperiment  : public Experiment<std::string>{
    
public:
    
    GA_SystemExperiment(
            const std::string id, 
            _agentsTasksAllocator&, 
            _ga_token& token, 
            const _taskMap& taskMap,
            unsigned cell_size = 0, 
            unsigned timestep = 0);
    
    
    GA_SystemExperiment(const GA_SystemExperiment& other);

    
    virtual void run();
    
    virtual ~GA_SystemExperiment();
    
private:
    
    _ga_system system;
    _ga_token& token;
    const _taskMap& taskMap;
    const unsigned cell_size = 0, timestep = 0;

};

#endif /* GA_SYSTEMEXPERIMENT_H */

