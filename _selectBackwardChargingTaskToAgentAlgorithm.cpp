/* 
 * File:   _selectBackwardChargingTaskToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 12 de setembro de 2023, 15:32
 */

#include "_selectBackwardChargingTaskToAgentAlgorithm.h"
#include <vector>
#include "_taskPathToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"
#include "_thresholdAlgorithm.h"
#include "_token.h"
#include "_agent.h"
#include "_task.h"
#include "_stepPath.h"

_selectBackwardChargingTaskToAgentAlgorithm::_selectBackwardChargingTaskToAgentAlgorithm(
        const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
        _taskIndexerAlgorithm& taskIndexerAlgorithm,
        _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
        float delivery_threshold) :
            _selectBackwardTaskToAgentAlgorithm(
                taskPathToAgentAlgorithm,
                taskIndexerAlgorithm,
                delivery_threshold),
        endpointIndexerAlgorithm(endpointIndexerAlgorithm) {}

_selectBackwardChargingTaskToAgentAlgorithm::_selectBackwardChargingTaskToAgentAlgorithm(
        const _selectBackwardChargingTaskToAgentAlgorithm& other) :
            _selectBackwardTaskToAgentAlgorithm(other),
            endpointIndexerAlgorithm(endpointIndexerAlgorithm) {}

bool _selectBackwardChargingTaskToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _task& originalTask,
        _task& selectedTask,
        _task& pendingTask,
        _stepPath& selectedPath) const {

    _thresholdAlgorithm thresholdAlgorithm(token.getEndpointsDistanceAlgorithm());

    bool ret = false;

    std::vector<_task> vtask;

    token.listPendingTasks([&vtask, &token, agent, this](const _task & task) {

        this->taskIndexerAlgorithm.solve(token, task, agent, vtask);

        return false;

    });

    bool aux = false;
    _task auxTask;
    _stepPath auxPath;

    for (auto task : vtask) {

        ret = true;

        token.listConstAgents([task, &ret, agent](const _agent& otherAgent) {

            if (otherAgent.id() != agent.id()) {

                if (otherAgent.goalSite().match(task.getPickup()) || otherAgent.goalSite().match(task.getDelivery())) {

                    ret = false;

                    return true;

                }

            }

            return false;

        });

        if (ret) {

            originalTask = task;

            _stepPath taskPath(selectedPath);

            _stepSite pickupSite, deliverySite;

            ret = taskPathToAgentAlgorithm.solve(token, agent, originalTask, taskPath, pickupSite, deliverySite);

            if (ret) {

                //Guarda para uso futuro
                aux = true;
                auxTask = task;
                auxPath = taskPath;
                
//                if(task.id() < 0) continue;

                taskPath.backward(
                        [&ret, &token, agent, &taskPath, thresholdAlgorithm,
                        pickupSite, deliverySite, &originalTask,
                        &selectedTask, &pendingTask, &selectedPath, this](const _stepSite & site) {

                            if (site.step_match(pickupSite)) return true;

                            if (token.isTaskDeliveryEndpoint(site)) {

                                ret = true;

                                token.listConstAgents([&ret, site, agent](const _agent& otherAgent) { // verifica se o task endpoint está disponível

                                    if (otherAgent.id() != agent.id()) {

                                        if (otherAgent.goalSite().match(site)) {

                                            ret = false;

                                            return true;

                                        }

                                    }

                                    return false;

                                });

                                if (ret) {

                                    unsigned step = site.GetStep() - pickupSite.GetStep();

                                    ret = thresholdAlgorithm.solve(pickupSite, site, step, this->delivery_threshold);

                                    if (ret) {

                                        std::vector<_site> vsite;

                                        token.listChargingEndpoints([&vsite, &token, deliverySite, this](const _site & chargingEndpoint) {

                                            this->endpointIndexerAlgorithm.solve(token, chargingEndpoint, deliverySite, vsite);

                                            return false;

                                        });

                                        for (auto endpoint : vsite) {

                                            ret = true;

                                            if (ret) {

                                                token.listConstAgents([endpoint, agent, &ret](const _agent & otherAgent) {

                                                    if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents

                                                        ret = false;

                                                        return true;

                                                    }

                                                    return false;

                                                });

                                            }

                                            if (ret) {

                                                _stepPath chargingTaskPath(site);

                                                ret = taskPathToAgentAlgorithm.getStepPathAlgorithm().solve(token, agent, chargingTaskPath, endpoint);

                                                if (ret) {

                                                    selectedTask.setPickup(pickupSite);
                                                    selectedTask.setDelivery(site);

                                                    _stepPath backwardTaskPath;

                                                    taskPath.forward([site, &backwardTaskPath](const _stepSite & tsite) {

                                                        backwardTaskPath.progress(tsite);

                                                        if (tsite.step_match(site)) {

                                                            return true;

                                                        }

                                                        return false;

                                                    });

                                                    chargingTaskPath.pop();
                                                    backwardTaskPath.progress(chargingTaskPath);

                                                    ret = agent.isAbleToFulfillChargingTaskPath(token.getMap(), selectedTask, backwardTaskPath);

                                                    if (ret) {

                                                        pendingTask.setPickup(site);
                                                        pendingTask.setDelivery(deliverySite);

                                                        backwardTaskPath.pop();
                                                        selectedPath.progress(backwardTaskPath);

                                                        return true;

                                                    }

                                                }

                                            }

                                        }

                                    }

                                }

                            }

                            return false;

                        });

            }

        }

        if (ret) return true;

    }

    if (aux) {

        std::vector<_site> vsite;

        token.listChargingEndpoints([&vsite, &token, auxTask, this](const _site & chargingEndpoint) {

            this->endpointIndexerAlgorithm.solve(token, chargingEndpoint, auxTask.getDelivery(), vsite);

            return false;

        });

        for (auto endpoint : vsite) {

            aux = true;

            if (aux) {

                token.listConstAgents([endpoint, agent, &aux](const _agent& otherAgent) {

                    if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents

                        aux = false;

                        return true;

                    }

                    return false;

                });

            }

            if (aux) {

                _stepPath chargingPath(auxPath.goalSite());

                aux = taskPathToAgentAlgorithm.getStepPathAlgorithm().solve(token, agent, chargingPath, endpoint);

                if (aux) {

                    chargingPath.pop();
                    _stepPath path(auxPath);
                    path.progress(chargingPath);

                    aux = agent.isAbleToFulfillChargingTaskPath(token.getMap(), auxTask, path);

                    if (aux) {
                        
                        originalTask = auxTask;
                        selectedTask = auxTask;

                        path.pop();
                        selectedPath.progress(path);

                        return true;

                    }

                }

            }

        }

    }

    return false;

}