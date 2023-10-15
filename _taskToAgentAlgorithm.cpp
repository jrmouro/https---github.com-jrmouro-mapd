/* 
 * File:   _taskToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 26 de setembro de 2023, 14:13
 */

#include "_taskToAgentAlgorithm.h"
#include "_token.h"
#include "_taskPathToAgentAlgorithm.h"

_taskToAgentAlgorithm::_taskToAgentAlgorithm(
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm) :
        taskPathToAgentAlgorithm(taskPathToAgentAlgorithm) {}
    
_taskToAgentAlgorithm::_taskToAgentAlgorithm(const _taskToAgentAlgorithm& other) :
        taskPathToAgentAlgorithm(other.taskPathToAgentAlgorithm) { }

_taskToAgentAlgorithm::~_taskToAgentAlgorithm(){}
    
bool _taskToAgentAlgorithm::solve(
        const _token& token, 
        const _agent& agent, 
        const _task& task, 
        _stepPath& taskPath, 
        _stepSite& pickupSite,
        _stepSite& deliverySite) const{
    
    bool flag = true;

    token.listConstAgents([task, &flag, agent](const _agent& otherAgent) {

        if (otherAgent.id() != agent.id()) {

            if (otherAgent.goalSite().match(task.getPickup()) || otherAgent.goalSite().match(task.getDelivery())) {

                flag = false;

                return true;

            }

        }

        return false;

    });

    if (flag) {
        
        _stepSite pickupSiteAux, deliverySiteAux;
        
        _stepPath taskPathAux(taskPath);

        flag = taskPathToAgentAlgorithm.solve(token, agent, task, taskPathAux, pickupSiteAux, deliverySiteAux);

        if (flag) {

            flag = agent.isAbleToFulfillTaskPath(token.getMap(), task, taskPathAux);

            if (flag) {

                taskPathAux.pop();
                taskPath.progress(taskPathAux);                
            
                pickupSite = pickupSiteAux;
                deliverySite = deliverySiteAux;

                return true;

            }

        }

    }
    
    return false;
    
}
