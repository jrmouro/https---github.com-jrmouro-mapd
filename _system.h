/* 
 * File:   _system.h
 * Author: ronaldo
 *
 * Created on 15 de agosto de 2023, 09:03
 */

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <vector>
#include "MapdException.h"
#include "InstanceMAPD.h"
#include "_token.h"
#include "TaskCarryThresholdToken.h"
#include "TaskThresholdToken.h"
#include "CarryThresholdToken.h"
#include "C_TaskCarryThresholdToken.h"
#include "_agent.h"

class _system {
public:
    
    enum TokenType{
        tp,
        taskThreshold_tp,
        carryThreshold_tp,
        taskCarryThreshold_tp,
        c_taskCarryThreshold_tp,
    };
    
    _system(){}
    
    _system(
            TokenType tokenType, 
            const InstanceMAPD& instanceMAPD, 
            float task_threshold = .0f, 
            float carry_threshold = .0f,
            int currentEnergyLevel, 
            int maximumEnergyLevel,
            int chargingEnergyLevel,
            int criticalEnergyLevel){        
        reset(tokenType, instanceMAPD, task_threshold, carry_threshold, currentEnergyLevel, maximumEnergyLevel, chargingEnergyLevel, criticalEnergyLevel);        
    }
    
    _system(const _system& other) :
        map(new _map(*other.map)), 
        stepMap(new _stepMap(*other.stepMap)), 
        taskMap(new _taskMap(*other.taskMap)), 
        token(new _token(*other.token)),
        endpointsDistanceAlgorithm(new _endpointsDistanceAlgorithm(*other.endpointsDistanceAlgorithm)),
        endpoints(new std::vector<_site>(*other.endpoints)),
        botsEndpoints(new std::vector<_site>(*other.botsEndpoints)){ }


    virtual ~_system(){
    
        if(token != nullptr) {
            delete token;
            delete map;
            delete taskMap;
            delete stepMap;
            delete endpointsDistanceAlgorithm;
            delete endpoints;
            delete botsEndpoints;
        }
    
    }
    
    
    
    void reset(
        TokenType tokenType, 
        const InstanceMAPD& instanceMAPD, 
        float task_threshold = .0f, 
        float carry_threshold = .0f,
        int currentEnergyLevel, 
        int maximumEnergyLevel,
        int chargingEnergyLevel,
        int criticalEnergyLevel){
        
        if(token != nullptr) {
            delete token;
            delete map;
            delete taskMap;
            delete stepMap;
            delete endpointsDistanceAlgorithm;
            delete endpoints;
            delete botsEndpoints;
        }
        
        botsEndpoints = new std::vector<_site>();
        
        endpoints = new std::vector<_site>(instanceMAPD.getEndpoints());
        
        endpointsDistanceAlgorithm = new _endpointsDistanceAlgorithm(instanceMAPD.getMap(), instanceMAPD.getEndpoints());
        
        lastStepTask = instanceMAPD.getLastStepTask();
        
        map = new _map(instanceMAPD.getMap());
        
        stepMap = new _stepMap(instanceMAPD.getStepMap());
        
        taskMap = new _taskMap(instanceMAPD.getTaskMap());
        
        switch(tokenType){
            case tp:
                token = new _token(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm, task_threshold, carry_threshold);
                break;
            case taskThreshold_tp:
                token = new TaskThresholdToken(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm, task_threshold, carry_threshold);
                break;
            case carryThreshold_tp:
                token = new CarryThresholdToken(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm, task_threshold, carry_threshold);
                break;
            case taskCarryThreshold_tp:
                token = new TaskCarryThresholdToken(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm, task_threshold, carry_threshold);
                break;
            case c_taskCarryThreshold_tp:
                token = new C_TaskCarryThresholdToken(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm, task_threshold, carry_threshold);
                break;
        }
        
        instanceMAPD.listBotsEndPoints(
                [currentEnergyLevel, maximumEnergyLevel, chargingEnergyLevel, criticalEnergyLevel, this]
                (unsigned id, const _site& site){
                        
            this->token->addAgent(_agent(id, _stepSite(0, site.GetRow(), site.GetColunm()), currentEnergyLevel, maximumEnergyLevel, chargingEnergyLevel, criticalEnergyLevel));
            this->botsEndpoints->push_back(site);
            
            return false;
            
        });            
        
    }
    
    void step(){ 
        
        if(token!=nullptr){
            
            _step();
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "uninitialized instance";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
    void run(){
      
        if(token != nullptr){
            
            run();
            
        }else{
            
            try {
                std::ostringstream stream;
                stream << "uninitialized instance";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
                
    }
    
    const _endpointsDistanceAlgorithm& getEndpointsDistanceAlgorithm() const {
        return *endpointsDistanceAlgorithm;
    }

    unsigned getLastStepTask() const {
        return lastStepTask;
    }

    _stepMap& getStepMap(){
        return *stepMap;
    }

    _token& getToken(){
        return *token;
    }
        
    void listEndpoints(const std::function<bool(const _site&)> function)const{
        
        for (auto endpoint : *endpoints) {
            
            if(function(endpoint)) return;

        }
        
    }
    
    void listChargingEndpoints(const std::function<bool(const _site&)> function)const{
        
        for (auto endpoint : *botsEndpoints) {
            
            if(function(endpoint)) return;

        }
        
    }
         

    
private:
    unsigned lastStepTask = 0;
    _map* map = nullptr;
    _stepMap* stepMap = nullptr;
    _taskMap* taskMap = nullptr;
    _token* token = nullptr;
    std::vector<_site> *endpoints = nullptr, *botsEndpoints = nullptr;
    _endpointsDistanceAlgorithm *endpointsDistanceAlgorithm = nullptr;
    
    void _step(){ 
        
        if(
            token->getCurrentStep() < stepMap->getStep_size() &&
            (   
                token->getCurrentStep() < lastStepTask 
                || !token->anyPendingTask()
                || !token->anyAssignedTask()
                || !token->anyRunningTask()
            )){
            
            taskMap->listTasksByStep(token->getCurrentStep(), [this](const _task& task){
                
                this->token->addPendingTask(task);
                
                return false;
                
            });
            
            token->listAgents([this](_agent& agent){
                
                agent.receive(*this);
                
                return false;
                
            });
                    
            token->listAgents([this](_agent& agent){
                
                agent.move(*this);
                
                return false;
                
            });
            
            token->stepping();
                        
        }
        
    }
    
    void _run(){
      
        unsigned step = token->getCurrentStep();
        
        while(
            token->getCurrentStep() < stepMap->getStep_size() &&
            (   
                token->getCurrentStep() < lastStepTask 
                || !token->anyPendingTask()
                || !token->anyAssignedTask()
                || !token->anyRunningTask()
            )){
            
                taskMap->listTasksByStep(token->getCurrentStep(), [this](const _task& task){

                    this->token->addPendingTask(task);

                    return false;

                });

                token->listAgents([this](_agent& agent){

                    agent.receive(*this);

                    return false;

                });

                token->listAgents([this](_agent& agent){

                    agent.move(*this);

                    return false;

                });

                token->stepping();
                        
        }
                
    }

};

#endif /* _SYSTEM_H */

