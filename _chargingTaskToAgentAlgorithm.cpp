/* 
 * File:   _chargingTaskToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 1 de outubro de 2023, 09:04
 */

#include "_chargingTaskToAgentAlgorithm.h"
#include "_token.h"
#include "_taskPathToAgentAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"

_chargingTaskToAgentAlgorithm::_chargingTaskToAgentAlgorithm(
    const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm, 
    _endpointIndexerAlgorithm& endpointIndexerAlgorithm) :
        taskPathToAgentAlgorithm(taskPathToAgentAlgorithm), 
        endpointIndexerAlgorithm(endpointIndexerAlgorithm) { }


_chargingTaskToAgentAlgorithm::_chargingTaskToAgentAlgorithm(
    const _chargingTaskToAgentAlgorithm& other) :
        taskPathToAgentAlgorithm(other.taskPathToAgentAlgorithm), 
        endpointIndexerAlgorithm(other.endpointIndexerAlgorithm) { }

_chargingTaskToAgentAlgorithm::~_chargingTaskToAgentAlgorithm(){}

void _chargingTaskToAgentAlgorithm::setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm){
    endpointIndexerAlgorithm = endpointIndexerAlgorithm;
}
    
bool _chargingTaskToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        const _task& task,
        _stepPath& selectedPath,
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

        _stepPath taskPath(selectedPath);

        _stepSite pickupSite, deliverySite;

        flag = taskPathToAgentAlgorithm.solve(token, agent, task, taskPath, pickupSite, deliverySite);

        if (flag) {

            std::vector<_site> vsite;

            token.listChargingEndpoints([&vsite, &token, deliverySite, this](const _site & chargingEndpoint) {

                this->endpointIndexerAlgorithm.solve(token, chargingEndpoint, deliverySite, vsite);

                return false;

            });

            for (auto endpoint : vsite) {

                flag = true;

                if (flag) {

                    token.listConstAgents([endpoint, agent, &flag](const _agent& otherAgent) {

                        if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents

                            flag = false;

                            return true;

                        }

                        return false;

                    });

                }

                if (flag) {

                    _stepPath chargingPath(deliverySite);

                    flag = taskPathToAgentAlgorithm.getStepPathAlgorithm().solve(token, agent, chargingPath, endpoint);

                    if (flag) {

                        chargingPath.pop();
                        _stepPath path(taskPath);
                        path.progress(chargingPath);

                        flag = agent.isAbleToFulfillChargingTaskPath(token.getMap(), task, path);

                        if (flag) {

                            path.pop();
                            selectedPath.progress(path);

                            return true;

                        }

                    } 

                }

            }            

        } 

    }
    
    return false;

}