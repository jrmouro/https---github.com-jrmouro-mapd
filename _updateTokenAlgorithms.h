/* 
 * File:   _updateTokenAlgorithms.h
 * Author: ronaldo
 *
 * Created on 7 de setembro de 2023, 03:58
 */

#ifndef _UPDATETOKENALGORITHMS_H
#define _UPDATETOKENALGORITHMS_H

#include "_stepAstarAlgorithm.h"
#include "_updateTrivialPathToAgentAlgorithm.h"
#include "_taskPathToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"
#include "_updateTaskToAgentAlgorithm.h"
#include "_updateEndpointToAgentAlgorithm.h"
#include "_selectChargingTaskToAgentThresholdAlgorithm.h"
#include "_selectTaskToAgentThresholdAlgorithm.h"
#include "_selectTaskToAgentAlgorithm.h"

class _pathToAgentAlgorithm;
class _taskPathToAgentAlgorithm;
class _selectChargingTaskToAgentAlgorithm;
class _selectEndpointToAgentAlgorithm;
class _selectEndpointToAgentAlgorithm;
class _updateEndpointToAgentAlgorithm;

class _updateTokenAlgorithms {
public:
            
    static _updateTokenAlgorithms* getInstance(
            float pickup_threshold = .0f,
            float delivery_threshold = .0f){
        
        if(instance == nullptr){
            
            instance = new _updateTokenAlgorithms(pickup_threshold, delivery_threshold);
            
        }
        
        instance->selectTaskToAgentThresholdAlgorithm->setDelivery_threshold(delivery_threshold);
        instance->selectTaskToAgentThresholdAlgorithm->setPickup_threshold(pickup_threshold);
        
        instance->selectChargingTaskToAgentThresholdAlgorithm->setDelivery_threshold(delivery_threshold);
        instance->selectChargingTaskToAgentThresholdAlgorithm->setPickup_threshold(pickup_threshold);
                        
        return instance;
        
    }
            
    static _updateTokenAlgorithms* getInstance(
            const _taskIndexerAlgorithm& taskIndexerAlgorithm,
            float pickup_threshold = .0f,
            float delivery_threshold = .0f){
        
        if(instance == nullptr){
            
            instance = new _updateTokenAlgorithms(taskIndexerAlgorithm, pickup_threshold, delivery_threshold);
            
        }
        
        instance->selectTaskToAgentThresholdAlgorithm->setDelivery_threshold(delivery_threshold);
        instance->selectTaskToAgentThresholdAlgorithm->setPickup_threshold(pickup_threshold);
        
        instance->selectChargingTaskToAgentThresholdAlgorithm->setDelivery_threshold(delivery_threshold);
        instance->selectChargingTaskToAgentThresholdAlgorithm->setPickup_threshold(pickup_threshold);
                        
        return instance;
        
    }
    
    static void deleteInstance(){
        
        if(instance != nullptr){
            
            delete instance;
            instance = nullptr;
            
        }
        
    }
    
    virtual ~_updateTokenAlgorithms();
    
    const _updateTrivialPathToAgentAlgorithm& getUpdateTrivialPathToAgentAlgorithm() const;    
    
    const _updateEndpointToAgentAlgorithm& getUpdateRestEndpointToAgentAlgorithm() const;
    const _updateEndpointToAgentAlgorithm& getUpdateChargingEndpointToAgentAlgorithm() const;
    
    const _updateTaskToAgentAlgorithm& getUpdateTaskToAgentAlgorithm() const;    
    const _updateTaskToAgentAlgorithm& getUpdateChargingTaskToAgentAlgorithm() const;
    
    const _updateTaskToAgentAlgorithm& getUpdateTaskToAgentThresholdAlgorithm() const;    
    const _updateTaskToAgentAlgorithm& getUpdateChargingTaskToAgentThresholdAlgorithm() const;
    

private:
    
    _updateTokenAlgorithms(
            float pickup_threshold,
            float delivery_threshold);
    
    _updateTokenAlgorithms(
            const _taskIndexerAlgorithm& taskIndexerAlgorithm,
            float pickup_threshold,
            float delivery_threshold);
        
    static _updateTokenAlgorithms* instance;
    
    
    _taskIndexerAlgorithm *taskIndexerAlgorithm; 
    
    _endpointIndexerAlgorithm* endpointIndexerAlgorithm;
    
    _stepPathAlgorithm* stepPathAlgorithm; 
    _pathToAgentAlgorithm* pathToAgentAlgorithm;
    _taskPathToAgentAlgorithm* taskPathToAgentAlgorithm;
    
    
    _selectTaskToAgentAlgorithm* selectTaskToAgentAlgorithm;
    _selectChargingTaskToAgentAlgorithm* selectChargingTaskToAgentAlgorithm;   
    
    _selectTaskToAgentThresholdAlgorithm* selectTaskToAgentThresholdAlgorithm;    
    _selectChargingTaskToAgentThresholdAlgorithm* selectChargingTaskToAgentThresholdAlgorithm;
    
    _selectEndpointToAgentAlgorithm
            * selectRestEndpointToAgentAlgorithm,
            * selectChargingEndpointToAgentAlgorithm;
    
    
    _updateTrivialPathToAgentAlgorithm* updateTrivialPathToAgentAlgorithm;  
    
    _updateTaskToAgentAlgorithm
            *updateTaskToAgentAlgorithm, 
            *updateChargingTaskToAgentAlgorithm, 
            *updateTaskToAgentThresholdAlgorithm,
            *updateChargingTaskToAgentThresholdAlgorithm;
    
    _updateEndpointToAgentAlgorithm
            * updateRestEndpointToAgentAlgorithm,
            * updateChargingEndpointToAgentAlgorithm;
    
    
};

#endif /* _UPDATETOKENALGORITHMS_H */

