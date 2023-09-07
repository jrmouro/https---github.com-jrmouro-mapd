/* 
 * File:   _updateTaskToAgentThresholdAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 17:32
 */

#ifndef _UPDATETASKTOAGENTTHRESHOLDALGORITHM_H
#define _UPDATETASKTOAGENTTHRESHOLDALGORITHM_H

#include "_selectTaskToAgentThresholdAlgorithm.h"

class _updateTaskToAgentThresholdAlgorithm {
    
public:
    
    _updateTaskToAgentThresholdAlgorithm(
            const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
            const _taskIndexerAlgorithm& taskIndexerAlgorithm,
            float pickup_threshold,
            float delivery_threshold):selectTaskToAgentThresholdAlgorithm(
                taskPathToAgentAlgorithm,
                taskIndexerAlgorithm,
                pickup_threshold,
                delivery_threshold){}
    
    _updateTaskToAgentThresholdAlgorithm(const _updateTaskToAgentThresholdAlgorithm& orig):
    selectTaskToAgentThresholdAlgorithm(orig.selectTaskToAgentThresholdAlgorithm){}
    
    virtual ~_updateTaskToAgentThresholdAlgorithm(){ }
    
    virtual bool solve(
            _token& token, 
            _agent& agent) const;
    
private:
    
    _selectTaskToAgentThresholdAlgorithm selectTaskToAgentThresholdAlgorithm;
    
};

#endif /* _UPDATETASKTOAGENTTHRESHOLDALGORITHM_H */

