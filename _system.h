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
#include "_agent.h"

class _system {
public:
    
    _system(){}
    
    _system(const InstanceMAPD& instanceMAPD){        
        reset(instanceMAPD);        
    }
    
    _system(const _system& other) :
        map(new _map(*other.map)), 
        stepMap(new _stepMap(*other.stepMap)), 
        taskMap(new TaskMap(*other.taskMap)), 
        token(new _token(*other.token)) { }


    virtual ~_system(){
    
        if(token != nullptr) {
            delete token;
            delete map;
            delete taskMap;
            delete stepMap;
        }
    
    }
    
    void reset(const InstanceMAPD& instanceMAPD){
        
        if(token != nullptr) {
            delete token;
            delete map;
            delete taskMap;
            delete stepMap;
            delete endpointsDistanceAlgorithm;
            delete endpoints;
        }
        
        endpoints = new std::vector<_site>(instanceMAPD.getEndpoints());
        
        endpointsDistanceAlgorithm = new _endpointsDistanceAlgorithm(instanceMAPD.getMap(), instanceMAPD.getEndpoints());
        
        lastStepTask = instanceMAPD.getLastStepTask();
        
        map = new _map(instanceMAPD.getMap());
        
        stepMap = new _stepMap(instanceMAPD.getStepMap());
        
        taskMap = new TaskMap(instanceMAPD.getTaskMap());
        
        token = new _token();
        
        instanceMAPD.listBotsEndPoints([this](unsigned id, const _site& site){
                        
            this->token->addAgent(_agent(id, _stepSite(0, site.GetRow(), site.GetColunm())));
            
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

//    const _map& getMap() const {
//        return *map;
//    }

    _stepMap& getStepMap(){
        return *stepMap;
    }

    _token& getToken(){
        return *token;
    }
    
//    const std::vector<_site> getEndpoints() const {
//        return *endpoints;
//    }
    
    void listEndpoints(const std::function<bool(const _site&)> function)const{
        
        for (auto endpoint : *endpoints) {
            
            if(function(endpoint)) return;

        }
        
    }
         

    
private:
    unsigned lastStepTask = 0;
    _map* map = nullptr;
    _stepMap* stepMap = nullptr;
    TaskMap* taskMap = nullptr;
    _token* token = nullptr;
    std::vector<_site>* endpoints = nullptr;
    _endpointsDistanceAlgorithm *endpointsDistanceAlgorithm = nullptr;
    
    void _step(){ 
        
        if(
            token->getCurrentStep() < stepMap->getStep_size() &&
            (   
                token->getCurrentStep() < lastStepTask 
                || !token->anyPendingTask()
                || !token->anyOpenTask()
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
                || !token->anyOpenTask()
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

