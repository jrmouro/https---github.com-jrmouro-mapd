/* 
 * File:   _selectTaskToAgentThresholdAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 05:28
 */

#ifndef _SELECTTASKTOAGENTTHRESHOLDALGORITHM_H
#define _SELECTTASKTOAGENTTHRESHOLDALGORITHM_H


//class _token;
//class _agent; 
//class _task;
//class _stepPath;
//class _thresholdAlgorithm;
//class _taskPathToAgentAlgorithm;
#include "_selectTaskToAgentAlgorithm.h"

class _selectTaskToAgentThresholdAlgorithm : public _selectTaskToAgentAlgorithm {
public:

    _selectTaskToAgentThresholdAlgorithm(
            const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
            _taskIndexerAlgorithm& taskIndexerAlgorithm,
            float pickup_threshold,
            float delivery_threshold);

    _selectTaskToAgentThresholdAlgorithm(
            const _selectTaskToAgentThresholdAlgorithm& other);

    virtual ~_selectTaskToAgentThresholdAlgorithm() { }

    virtual bool solve(const _token&, const _agent&, _task&, _stepPath&) const;
    
    void setDelivery_threshold(float delivery_threshold);

    void setPickup_threshold(float pickup_threshold);

protected:

    float pickup_threshold = .0f, delivery_threshold = .0f;

};

#endif /* _SELECTTASKTOAGENTTHRESHOLDALGORITHM_H */

