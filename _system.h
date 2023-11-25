/* 
 * File:   _system.h
 * Author: ronaldo
 *
 * Created on 15 de agosto de 2023, 09:03
 */

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <cmath>
#include <vector>
#include "MapdException.h"
#include "InstanceMAPD.h"
#include "_token.h"
#include "_agent.h"

class _system{
    
public:    
    
    bool step(const _taskMap& taskMap, _token& token) const { 
        
        unsigned lastStepTask = taskMap.getLastTask() + 1;
        
        if(!token.isFinalizedCondition(lastStepTask)){
            
//            token.error_site_collision_check();
//            token.error_edge_collision_check();
            
            int i = 0;
            
            taskMap.listTasksByStep(token.getCurrentStep(), [&token](const _task& task){
                
                token.addPendingTask(task);
                
                return false;
                
            });
            
            i = 0;
            
            token.listAgents([this, &token](_agent& agent){
                
                agent.receive(token);
                
                return false;
                
            });
            
            i = 0;
                    
            token.listAgents([this, &token](_agent& agent){
                
                agent.move(token);
                
                return false;
                
            });
            
            i = 0;
            
            token.stepping();
            
            return true;
                        
        }
        
        return false;
        
    }
    
    void run(const _taskMap& taskMap, _token& token){
      
        unsigned lastStepTask = taskMap.getLastTask() + 1;
//        unsigned step = token.getCurrentStep();
        
        while(!token.isFinalizedCondition(lastStepTask)){
            
//            token.error_site_collision_check();
//            token.error_edge_collision_check();
            
                taskMap.listTasksByStep(token.getCurrentStep(), [&token](const _task& task){

                    token.addPendingTask(task);

                    return false;

                });

                token.listAgents([this, &token](_agent& agent){

                    agent.receive(token);

                    return false;

                });

                token.listAgents([this, &token](_agent& agent){

                    agent.move(token);

                    return false;

                });

                token.stepping();
                        
        }
                
    }

};

#endif /* _SYSTEM_H */

