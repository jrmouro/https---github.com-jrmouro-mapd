/* 
 * File:   _selectTaskToAgentThresholdAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 05:28
 */

#ifndef _SELECTTASKTOAGENTTHRESHOLDALGORITHM_H
#define _SELECTTASKTOAGENTTHRESHOLDALGORITHM_H

#include "_selectTaskToAgentAlgorithm.h"
#include "_endPointsDistanceAlgorithm.h"
#include "_taskPathToAgentAlgorithm.h"
#include "_thresholdAlgorithm.h"


class _selectTaskToAgentThresholdAlgorithm : public _selectTaskToAgentAlgorithm {
public:

    _selectTaskToAgentThresholdAlgorithm(
            const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
            const _taskIndexerAlgorithm& taskIndexerAlgorithm,
            float pickup_threshold,
            float delivery_threshold) :
                _selectTaskToAgentAlgorithm(
                    taskPathToAgentAlgorithm, 
                    taskIndexerAlgorithm),
                pickup_threshold(pickup_threshold),
                delivery_threshold(delivery_threshold) { }

    _selectTaskToAgentThresholdAlgorithm(
            const _selectTaskToAgentThresholdAlgorithm& other) :
                _selectTaskToAgentAlgorithm(other),
                pickup_threshold(other.pickup_threshold),
                delivery_threshold(other.delivery_threshold) { }

    virtual ~_selectTaskToAgentThresholdAlgorithm() { }

    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            _task& selectedTask, 
            _stepPath& selectedPath) const;

private:

    float pickup_threshold = .0f, delivery_threshold = .0f;

};

#endif /* _SELECTTASKTOAGENTTHRESHOLDALGORITHM_H */

