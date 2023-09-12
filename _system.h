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
#include "ThresholdTokenPass.h"
#include "TokenPass.h"
#include "CarryThresholdToken.h"
#include "_agent.h"

class _system : public Writable{
public:
    
    enum TokenType{
        tokenPass,
        threshold_tokenPass,
        c_taskCarryThreshold_tp,
    };
        
    _system(
            TokenType tokenType, 
            std::string taskFilename, 
            std::string mapFilename,             
            int currentEnergyLevel, 
            int maximumEnergyLevel,
            int chargedEnergyLevel,
            int criticalEnergyLevel,
            float pickup_threshold, 
            float delivery_threshold){        
        reset(
                tokenType, 
                taskFilename, 
                mapFilename,                  
                currentEnergyLevel, 
                maximumEnergyLevel, 
                chargedEnergyLevel, 
                criticalEnergyLevel,
                pickup_threshold, 
                delivery_threshold);        
    }
    
    _system(const _system& other) :
        taskFilename(other.taskFilename), 
        mapFilename(other.mapFilename), 
        instanceMAPD(new InstanceMAPD(*other.instanceMAPD)),
        map(new _map(*other.map)), 
        stepMap(new _stepMap(*other.stepMap)), 
        taskMap(new _taskMap(*other.taskMap)), 
        token(other.token->getInstance()),
        endpointsDistanceAlgorithm(new _endpointsDistanceAlgorithm(*other.endpointsDistanceAlgorithm)),
        endpoints(new std::vector<_site>(*other.endpoints)),
        botsEndpoints(new std::vector<_site>(*other.botsEndpoints)){ }


    virtual ~_system(){
    
        if(token != nullptr) {
            delete instanceMAPD;
            delete token;
            delete map;
            delete taskMap;
            delete stepMap;
            delete endpointsDistanceAlgorithm;
            delete endpoints;
            delete botsEndpoints;
        }
    
    }
    
    InstanceMAPD& getInstanceMAPD() const {
        return *instanceMAPD;
    }

    
    virtual void writeHeader(std::ostream& fs) const {
        
        if(token != nullptr && this->instanceMAPD != nullptr){
            
            Writable::strWrite(*this, fs, "mapFilename", true); 
            Writable::strWrite(*this, fs, "taskFilename", true);
            this->instanceMAPD->writeHeader(fs);
            Writable::sepWrite(*this, fs);
            this->token->writeHeader(fs);
                        
        }else {
            
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
    
    virtual void writeRow(std::ostream& fs) const {
        
        if(token != nullptr && this->instanceMAPD != nullptr){
            
            Writable::strWrite(*this, fs, mapFilename, true); 
            Writable::strWrite(*this, fs, taskFilename, true);
            this->instanceMAPD->writeRow(fs);
            Writable::sepWrite(*this, fs);
            this->token->writeRow(fs);
                        
        }else {
            
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
    
    
    void reset(
        TokenType tokenType, 
        std::string taskFilename, 
        std::string mapFilename,        
        int currentEnergyLevel, 
        int maximumEnergyLevel,
        int chargedEnergyLevel,
        int criticalEnergyLevel,
        float pickup_threshold, 
        float delivery_threshold){
        
        this->mapFilename = mapFilename;
        this->taskFilename = taskFilename;
        
        
        
        if(token != nullptr) {
            delete instanceMAPD;
            delete token;
            delete map;
            delete taskMap;
            delete stepMap;
            delete endpointsDistanceAlgorithm;
            delete endpoints;
            delete botsEndpoints;
        }
        
        this->instanceMAPD = InstanceMAPD::load(mapFilename, taskFilename);
        
        botsEndpoints = new std::vector<_site>();
        
        endpoints = new std::vector<_site>(instanceMAPD->getEndpoints());
        
        endpointsDistanceAlgorithm = new _endpointsDistanceAlgorithm(instanceMAPD->getMap(), instanceMAPD->getEndpoints());
        
        lastStepTask = instanceMAPD->getLastStepTask();
        
        map = new _map(instanceMAPD->getMap());
        
        stepMap = new _stepMap(instanceMAPD->getStepMap());
        
        taskMap = new _taskMap(instanceMAPD->getTaskMap());
        
        switch(tokenType){
            case tokenPass:
                token = new TokenPass(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm);
                break;
            case threshold_tokenPass:
                token = new ThresholdTokenPass(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm, pickup_threshold, delivery_threshold);
                break;
//            case c_taskCarryThreshold_tp:
//                token = new C_TaskCarryThresholdToken(*map, *stepMap, *endpoints, *botsEndpoints, *endpointsDistanceAlgorithm, pickup_threshold, delivery_threshold);
//                break;
        }
        
        int inc = (maximumEnergyLevel - chargedEnergyLevel) / instanceMAPD->getNumBots();
        int count = 0;
        
        instanceMAPD->listBotsEndPoints(
                [currentEnergyLevel, maximumEnergyLevel, chargedEnergyLevel, criticalEnergyLevel, inc, &count, this]
                (unsigned id, const _site& site){
                        
            int ecurrent = std::min<int>(maximumEnergyLevel, currentEnergyLevel + inc*count);    
            this->token->addAgent(_agent(id, _stepSite(0, site.GetRow(), site.GetColunm()), ecurrent, maximumEnergyLevel, chargedEnergyLevel, criticalEnergyLevel));
            this->botsEndpoints->push_back(site);
            
            count++;
            
            return false;
            
        });            
        
    }
    
    bool step(){ 
        
        if(token!=nullptr){
            
            return _step();
            
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
        
        return false;
        
    }
    
    void run(){
      
        if(token != nullptr){
            
            _run();
            
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
    
    std::string taskFilename, mapFilename;
    
    InstanceMAPD* instanceMAPD = nullptr;
    _map* map = nullptr;
    _stepMap* stepMap = nullptr;
    _taskMap* taskMap = nullptr;
    _token* token = nullptr;
    std::vector<_site> *endpoints = nullptr, *botsEndpoints = nullptr;
    _endpointsDistanceAlgorithm *endpointsDistanceAlgorithm = nullptr;
    
    unsigned lastStepTask = 0;
    
    bool _step(){ 
        
        if(
            token->getCurrentStep() < stepMap->getStep_size() &&
            (   
                token->getCurrentStep() < lastStepTask 
                || !token->anyPendingTask()
                || !token->anyAssignedTask()
                || !token->anyRunningTask()
            )){
            
            token->error_site_collision_check();
            token->error_edge_collision_check();
            
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
            
            return true;
                        
        }
        
        return false;
        
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
            
            token->error_site_collision_check();
            token->error_edge_collision_check();
            
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

