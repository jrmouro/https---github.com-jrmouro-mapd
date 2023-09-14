/* 
 * File:   _taskPathToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 05:44
 */

#ifndef _TASKPATHTOAGENTALGORITHM_H
#define _TASKPATHTOAGENTALGORITHM_H

#include "_token.h"
#include "_agent.h"
#include "_task.h"
#include "_stepPath.h"
#include "_stepAstarAlgorithm.h"

class _taskPathToAgentAlgorithm {
    
public:
    
    _taskPathToAgentAlgorithm(const _stepPathAlgorithm& stepPathAlgorithm) :
    stepPathAlgorithm(stepPathAlgorithm) { }
    
    _taskPathToAgentAlgorithm(const _taskPathToAgentAlgorithm& other) :
    stepPathAlgorithm(other.stepPathAlgorithm) { }

    virtual ~_taskPathToAgentAlgorithm() { }
    
    virtual bool solve(
            const _token& token, 
            const _agent& agent, 
            const _task& task, 
            _stepPath& path, 
            _stepSite& pickupSite,
            _stepSite& deliverySite) const {
                
        bool flag = stepPathAlgorithm.solve(token, agent, path, task.getPickup());

        if (flag) {                       
                
            pickupSite = path.goalSite();

            flag = stepPathAlgorithm.solve(token, agent, path, task.getDelivery());

            if(flag){

                deliverySite = path.goalSite();

                return true;

            }

        } 

        return false;
        
    }
    
    const _stepPathAlgorithm& getStepPathAlgorithm() const {
        return stepPathAlgorithm;
    }
    
protected:
    
    const _stepPathAlgorithm& stepPathAlgorithm;

};

#endif /* _TASKPATHTOAGENTALGORITHM_H */

