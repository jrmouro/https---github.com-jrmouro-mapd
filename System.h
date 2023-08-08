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
    
    System(InstanceMAPD* instanceMAPD) :
    instanceMAPD(instanceMAPD) {}
    
    System(const System& other) :
    instanceMAPD(new InstanceMAPD(*other.instanceMAPD)) {}

    virtual ~System(){}
    
    void run(){
        
        std::vector<Agent> agents;
        
        instanceMAPD->instanceMap->listBots([agents&](unsigned id, const Site& site){
            
            agents.push_back(Agent(id, site));
            
            return false;
            
        });
        
        Token token(
            instanceMAPD->instanceMap->getSiteMap(),
            instanceMAPD->instanceMap->getBinaryMap(),
            agents);
        
        
        unsigned step = 0;
        
        while(
            step < instanceMAPD->instanceMap->getBinaryMap().getStep_size() &&
            (step < instanceMAPD->instanceTask->getLastStep() || !token.tasksEmpty())){
                                    
            instanceMAPD->instanceTask->getTaskMap().listTasksByStep(step, [token&](const Task& task){
                
                token.addTask(task);
                
                return false;
                
            });
            
            token.listAgents([token&](const Agent& agent){
                
                agent.receive(token);
                
                return false;
                
            });
                    
            token.listAgents([token&](const Agent& agent){
                
                agent.receive(token);
                
                return false;
                
            });
            
            token.setStep(step);
            
            step++;
            
        }
        
    }
    
private:
    
    InstanceMAPD* instanceMAPD;

};

#endif /* SYSTEM_H */

