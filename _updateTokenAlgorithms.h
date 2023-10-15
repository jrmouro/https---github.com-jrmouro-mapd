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
#include "_selectBackwardTaskToAgentAlgorithm.h"
#include "_selectBackwardChargingTaskToAgentAlgorithm.h"
#include "_selectTaskSwapToAgentAlgorithm.h"
#include "_updateBackwardTaskToAgentAlgorithm.h"
#include "_updateTaskSwapToAgentAlgorithm.h"
#include "_selectChargingTaskToAgentAlgorithm.h"
#include "_closerEndpointIndexerAlgorithm.h"
#include "_closerTaskIndexerAlgorithm.h"
#include "_closerTaskIndexerThresholdAlgorithm.h"

class _pathToAgentAlgorithm;
class _taskPathToAgentAlgorithm;
class _selectChargingTaskToAgentAlgorithm;
class _selectEndpointToAgentAlgorithm;
class _selectEndpointToAgentAlgorithm;
class _updateEndpointToAgentAlgorithm;
class _updateToAgentAlgorithm;

class _updateTokenAlgorithms {
public:
            
          
    static _updateTokenAlgorithms* getInstance(
            unsigned task_threshold = UINT_MAX,
            float pickup_threshold = .0f,
            float delivery_threshold = .0f){
        
        if(instance == nullptr){
            
            instance = new _updateTokenAlgorithms(task_threshold, pickup_threshold, delivery_threshold);
            
        } else {
            
            instance->taskIndexerAlgorithm->setTaskThreshold(task_threshold);
        
            instance->selectTaskToAgentThresholdAlgorithm->setDelivery_threshold(delivery_threshold);
            instance->selectTaskToAgentThresholdAlgorithm->setPickup_threshold(pickup_threshold);

            instance->selectChargingTaskToAgentThresholdAlgorithm->setDelivery_threshold(delivery_threshold);
            instance->selectChargingTaskToAgentThresholdAlgorithm->setPickup_threshold(pickup_threshold);

            instance->selectBackwardTaskToAgentAlgorithm->setDelivery_threshold(delivery_threshold);
            instance->selectBackwardChargingTaskToAgentAlgorithm->setDelivery_threshold(delivery_threshold);
                                        
        }
                        
        return instance;
        
    }
    
    static void deleteInstance(){
        
        if(instance != nullptr){
            
            delete instance;
            instance = nullptr;
            
        }
        
    }
    
    virtual ~_updateTokenAlgorithms();
    
    const _updateToAgentAlgorithm& getUpdateTrivialPathToAgentAlgorithm() const;    
    
    const _updateToAgentAlgorithm& getUpdateRestEndpointToAgentAlgorithm() const;
    const _updateToAgentAlgorithm& getUpdateChargingEndpointToAgentAlgorithm() const;
    
    const _updateToAgentAlgorithm& getUpdateTaskToAgentAlgorithm() const;    
    const _updateToAgentAlgorithm& getUpdateChargingTaskToAgentAlgorithm() const;
    
    const _updateToAgentAlgorithm& getUpdateTaskToAgentThresholdAlgorithm() const;    
    const _updateToAgentAlgorithm& getUpdateChargingTaskToAgentThresholdAlgorithm() const;
    
    const _updateToAgentAlgorithm& getUpdateBackwardTaskToAgentAlgorithm() const;
    const _updateToAgentAlgorithm& getUpdateBackwardChargingTaskToAgentAlgorithm() const;
    
    const _updateToAgentAlgorithm& getUpdateTaskSwapToAgentAlgorithm() const;
    

private:
    
    
//    _updateTokenAlgorithms();
        
    _updateTokenAlgorithms(
            unsigned task_threshold,
            float pickup_threshold,
            float delivery_threshold);
        
    static _updateTokenAlgorithms* instance;
    
    
    _closerTaskIndexerThresholdAlgorithm *taskIndexerAlgorithm; 
    
    _endpointIndexerAlgorithm* endpointIndexerAlgorithm;
    
    _stepPathAlgorithm* stepPathAlgorithm; 
    _pathToAgentAlgorithm* pathToAgentAlgorithm;
    _taskPathToAgentAlgorithm* taskPathToAgentAlgorithm;
        
    _selectTaskSwapToAgentAlgorithm* selectTaskSwapToAgentAlgorithm;
    
    _selectTaskToAgentAlgorithm* selectTaskToAgentAlgorithm;
    _selectChargingTaskToAgentAlgorithm* selectChargingTaskToAgentAlgorithm;   
    
    _selectTaskToAgentThresholdAlgorithm* selectTaskToAgentThresholdAlgorithm;    
    _selectChargingTaskToAgentThresholdAlgorithm* selectChargingTaskToAgentThresholdAlgorithm;
    
    _selectBackwardTaskToAgentAlgorithm* selectBackwardTaskToAgentAlgorithm;
    _selectBackwardChargingTaskToAgentAlgorithm* selectBackwardChargingTaskToAgentAlgorithm;
    
    _selectEndpointToAgentAlgorithm
            * selectRestEndpointToAgentAlgorithm,
            * selectChargingEndpointToAgentAlgorithm;
    
    
    _updateToAgentAlgorithm
            * updateTrivialPathToAgentAlgorithm,
            *updateTaskToAgentAlgorithm, 
            *updateChargingTaskToAgentAlgorithm, 
            *updateTaskToAgentThresholdAlgorithm,
            *updateChargingTaskToAgentThresholdAlgorithm,
            *updateRestEndpointToAgentAlgorithm,
            *updateChargingEndpointToAgentAlgorithm, 
            *updateBackwardTaskToAgentAlgorithm,
            *updateBackwardChargingTaskToAgentAlgorithm,
            *updateTaskSwapToAgentAlgorithm;
    
    
};

#endif /* _UPDATETOKENALGORITHMS_H */

