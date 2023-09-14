/* 
 * File:   TokenAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 7 de setembro de 2023, 03:58
 */

#include "_updateTokenAlgorithms.h"
#include "_selectChargingTaskToAgentAlgorithm.h"
#include "_updateTaskToAgentAlgorithm.h"
#include "_updateEndpointToAgentAlgorithm.h"
#include "_closerTaskIndexerAlgorithm.h"
#include "_closerEndpointIndexerAlgorithm.h"
#include "_pathToAgentAlgorithm.h"
#include "_selectChargingEndpointToAgentAlgorithm.h"

_updateTokenAlgorithms* _updateTokenAlgorithms::instance = nullptr;

_updateTokenAlgorithms::_updateTokenAlgorithms(
        const _taskIndexerAlgorithm& taskIndexerAlgorithm,
        float pickup_threshold,
        float delivery_threshold) : _updateTokenAlgorithms(pickup_threshold, delivery_threshold){

    delete this->taskIndexerAlgorithm;
    this->taskIndexerAlgorithm = taskIndexerAlgorithm.getInstance();
}

_updateTokenAlgorithms::_updateTokenAlgorithms(
        float pickup_threshold,
        float delivery_threshold) :
stepPathAlgorithm(new _stepAstarAlgorithm()),
updateTrivialPathToAgentAlgorithm(new _updateTrivialPathToAgentAlgorithm()),
taskIndexerAlgorithm(new _closerTaskIndexerAlgorithm()),
endpointIndexerAlgorithm(new _closerEndpointIndexerAlgorithm()) {

    pathToAgentAlgorithm = new _pathToAgentAlgorithm(
            *stepPathAlgorithm);

    taskPathToAgentAlgorithm = new _taskPathToAgentAlgorithm(
            *stepPathAlgorithm);
    
    selectRestEndpointToAgentAlgorithm = new _selectRestEndpointToAgentAlgorithm(
            *endpointIndexerAlgorithm,
            *pathToAgentAlgorithm);
        
    selectChargingEndpointToAgentAlgorithm = new _selectChargingEndpointToAgentAlgorithm(
            *endpointIndexerAlgorithm,
            *pathToAgentAlgorithm);

    selectTaskToAgentAlgorithm = new _selectTaskToAgentAlgorithm(
            *taskPathToAgentAlgorithm, *taskIndexerAlgorithm);
    
    selectChargingTaskToAgentAlgorithm = new _selectChargingTaskToAgentAlgorithm(
            *taskPathToAgentAlgorithm, 
            *taskIndexerAlgorithm, 
            *endpointIndexerAlgorithm);
    
    selectTaskToAgentThresholdAlgorithm = new _selectTaskToAgentThresholdAlgorithm(
            *taskPathToAgentAlgorithm, 
            *taskIndexerAlgorithm, 
            pickup_threshold,
            delivery_threshold);
    
    selectChargingTaskToAgentThresholdAlgorithm = new _selectChargingTaskToAgentThresholdAlgorithm(
            *taskPathToAgentAlgorithm, 
            *taskIndexerAlgorithm, 
            *endpointIndexerAlgorithm,
            pickup_threshold,
            delivery_threshold);
    
    selectBackwardTaskToAgentAlgorithm = new _selectBackwardTaskToAgentAlgorithm(
            *taskPathToAgentAlgorithm, 
            *taskIndexerAlgorithm, 
            delivery_threshold);
    
    selectBackwardChargingTaskToAgentAlgorithm = new _selectBackwardChargingTaskToAgentAlgorithm(
            *taskPathToAgentAlgorithm, 
            *taskIndexerAlgorithm, 
            *endpointIndexerAlgorithm,
            delivery_threshold);

    updateTaskToAgentAlgorithm = new _updateTaskToAgentAlgorithm(
            *selectTaskToAgentAlgorithm);

    updateChargingTaskToAgentAlgorithm = new _updateTaskToAgentAlgorithm(
            *selectChargingTaskToAgentAlgorithm);
    
    updateTaskToAgentThresholdAlgorithm = new _updateTaskToAgentAlgorithm(
            *selectTaskToAgentThresholdAlgorithm);

    updateChargingTaskToAgentThresholdAlgorithm = new _updateTaskToAgentAlgorithm(
            *selectChargingTaskToAgentThresholdAlgorithm);

    updateRestEndpointToAgentAlgorithm = new _updateEndpointToAgentAlgorithm(
            *selectRestEndpointToAgentAlgorithm);

    updateChargingEndpointToAgentAlgorithm = new _updateEndpointToAgentAlgorithm(
            *selectChargingEndpointToAgentAlgorithm);
    
    updateBackwardTaskToAgentAlgorithm = new _updateBackwardTaskToAgentAlgorithm(
            *selectBackwardTaskToAgentAlgorithm);
    
    updateBackwardChargingTaskToAgentAlgorithm = new _updateBackwardTaskToAgentAlgorithm(
            *selectBackwardChargingTaskToAgentAlgorithm);

    
}

_updateTokenAlgorithms::~_updateTokenAlgorithms() {

    delete stepPathAlgorithm;
    delete updateTrivialPathToAgentAlgorithm;
    delete taskPathToAgentAlgorithm;
    delete taskIndexerAlgorithm;
    delete endpointIndexerAlgorithm;
    delete pathToAgentAlgorithm;
    
    delete selectRestEndpointToAgentAlgorithm;
    delete selectChargingEndpointToAgentAlgorithm;
    
    delete selectTaskToAgentAlgorithm;
    delete selectChargingTaskToAgentAlgorithm;    
    delete selectTaskToAgentThresholdAlgorithm;    
    delete selectChargingTaskToAgentThresholdAlgorithm;    
    
    delete selectBackwardTaskToAgentAlgorithm;
    delete selectBackwardChargingTaskToAgentAlgorithm;
    
    
    delete updateTaskToAgentAlgorithm;
    delete updateChargingTaskToAgentAlgorithm;
    delete updateRestEndpointToAgentAlgorithm;
    delete updateChargingEndpointToAgentAlgorithm;
    delete updateTaskToAgentThresholdAlgorithm;
    delete updateChargingTaskToAgentThresholdAlgorithm;
    delete updateBackwardTaskToAgentAlgorithm;
    delete updateBackwardChargingTaskToAgentAlgorithm;

}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateTrivialPathToAgentAlgorithm() const {
    return *updateTrivialPathToAgentAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateRestEndpointToAgentAlgorithm() const {
    return *updateRestEndpointToAgentAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateChargingEndpointToAgentAlgorithm() const {
    return *updateChargingEndpointToAgentAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateTaskToAgentAlgorithm() const {
    return *updateTaskToAgentAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateChargingTaskToAgentAlgorithm() const {
    return *updateChargingTaskToAgentAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateTaskToAgentThresholdAlgorithm() const {
    return *updateTaskToAgentThresholdAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateChargingTaskToAgentThresholdAlgorithm() const {
    return *updateChargingTaskToAgentThresholdAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateBackwardTaskToAgentAlgorithm() const {
    return *updateBackwardTaskToAgentAlgorithm;
}

const _updateToAgentAlgorithm& _updateTokenAlgorithms::getUpdateBackwardChargingTaskToAgentAlgorithm() const {
    return *updateBackwardChargingTaskToAgentAlgorithm;
}