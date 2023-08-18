/* 
 * File:   _system.h
 * Author: ronaldo
 *
 * Created on 15 de agosto de 2023, 09:03
 */

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "InstanceMAPD.h"
#include "_token.h"
#include "_agent.h"

class _system {
public:
    
    _system(InstanceMAPD& instanceMAPD) :
    instanceMAPD(instanceMAPD) {}
    
    _system(const _system& other) :
    instanceMAPD(other.instanceMAPD) {}

    virtual ~_system(){}
    
    void step(_token& token){ 
        
        if(
            token.getCurrentStep() < instanceMAPD.instanceMap->getIntegerMap().getStep_size() &&
            (   
                token.getCurrentStep() < instanceMAPD.instanceTask->getLastStep() 
                || !token.anyPendingTask()
                || !token.anyOpenTask()
            )){
            
            instanceMAPD.instanceTask->getTaskMap().listTasksByStep(token.getCurrentStep(), [&token](const _task& task){
                
                token.addPendingTask(task);
                
                return false;
                
            });
            
            token.listAgents([&token](_agent& agent){
                
                agent.receive(token);
                
                return false;
                
            });
                    
            token.listAgents([&token](_agent& agent){
                
                agent.move(token);
                
                return false;
                
            });
            
            token.stepping();
                        
        }
        
    }
    
    void run(_token& token){
      
        unsigned step = token.getCurrentStep();
        
        while(
            token.getCurrentStep() < instanceMAPD.instanceMap->getIntegerMap().getStep_size() &&
            (   
                token.getCurrentStep() < instanceMAPD.instanceTask->getLastStep() 
                || !token.anyPendingTask()
                || !token.anyOpenTask()
            )){
                                  
                                    
            instanceMAPD.instanceTask->getTaskMap().listTasksByStep(token.getCurrentStep(), [&token](const _task& task){
                
                token.addPendingTask(task);
                
                return false;
                
            });
            
            token.listAgents([&token](_agent& agent){
                
                agent.receive(token);
                
                return false;
                
            });
                    
            token.listAgents([&token](_agent& agent){
                
                agent.move(token);
                
                return false;
                
            });
            
            token.stepping();
                        
        }
                
    }
    
private:
    
    InstanceMAPD& instanceMAPD;

};

#endif /* _SYSTEM_H */

