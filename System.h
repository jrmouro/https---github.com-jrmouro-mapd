/* 
 * File:   System.h
 * Author: ronaldo
 *
 * Created on 6 de agosto de 2023, 09:28
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "InstanceMAPD.h"
#include "Token.h"
#include "Agent.h"

class System {
public:
    
    System(InstanceMAPD& instanceMAPD) :
    instanceMAPD(instanceMAPD) {}
    
    System(const System& other) :
    instanceMAPD(other.instanceMAPD) {}

    virtual ~_system(){}
    
    void step(Token& token){ 
        
        if(
            token.getCurrentStep() < instanceMAPD.instanceMap->getIntegerMap().getStep_size() &&
            (   
                token.getCurrentStep() < instanceMAPD.instanceTask->getLastStep() 
                || !token.anyPendingTask()
                || !token.anyOpenTask()
            )){
            
            instanceMAPD.instanceTask->getTaskMap().listTasksByStep(token.getCurrentStep(), [&token](const Task& task){
                
                token.addPendingTask(task);
                
                return false;
                
            });
            
            token.listAgents([&token](Agent& agent){
                
                agent.receive(token);
                
                return false;
                
            });
                    
            token.listAgents([&token](Agent& agent){
                
                agent.move(token);
                
                return false;
                
            });
            
            token.stepping();
                        
        }
        
    }
    
    void run(Token& token){
      
        unsigned step = token.getCurrentStep();
        
        while(
            token.getCurrentStep() < instanceMAPD.instanceMap->getIntegerMap().getStep_size() &&
            (   
                token.getCurrentStep() < instanceMAPD.instanceTask->getLastStep() 
                || !token.anyPendingTask()
                || !token.anyOpenTask()
            )){
                                  
                                    
            instanceMAPD.instanceTask->getTaskMap().listTasksByStep(token.getCurrentStep(), [&token](const Task& task){
                
                token.addPendingTask(task);
                
                return false;
                
            });
            
            token.listAgents([&token](Agent& agent){
                
                agent.receive(token);
                
                return false;
                
            });
                    
            token.listAgents([&token](Agent& agent){
                
                agent.move(token);
                
                return false;
                
            });
            
            token.stepping();
                        
        }
                
    }
    
private:
    
    InstanceMAPD& instanceMAPD;

};

#endif /* SYSTEM_H */

