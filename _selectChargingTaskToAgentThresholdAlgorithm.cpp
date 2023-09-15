#include "_selectChargingTaskToAgentThresholdAlgorithm.h"
#include "_token.h"
#include "_thresholdAlgorithm.h"
#include "_taskPathToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"

bool _selectChargingTaskToAgentThresholdAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _task& selectedTask,
        _stepPath& selectedPath) const {

    _thresholdAlgorithm thresholdAlgorithm(token.getEndpointsDistanceAlgorithm());

    std::vector<_task> vtask;

    token.listPendingTasks([&vtask, &token, agent, this](const _task & task) {

        this->taskIndexerAlgorithm.solve(token, task, agent, vtask);

        return false;

    });
    
    bool aux = false;
    _task auxTask;
    _stepPath auxPath;

    for (auto task : vtask) {

        bool flag = true;

        token.listAgents([task, &flag, agent](const _agent & otherAgent) {

            if (otherAgent.id() != agent.id()) {

                if (otherAgent.goalSite().match(task.getPickup()) || otherAgent.goalSite().match(task.getDelivery())) {

                    flag = false;

                    return true;

                }

            }

            return false;

        });

        if (flag) {

            selectedTask = task;
            
            _stepPath taskPath(selectedPath);

            _stepSite pickupSite, deliverySite;

            flag = taskPathToAgentAlgorithm.solve(token, agent, selectedTask, taskPath, pickupSite, deliverySite);

            if (flag) {
                
                //Guarda para uso futuro
                aux = true;
                auxTask = task;
                auxPath = taskPath;

                if (flag) {

                    unsigned step = pickupSite.GetStep() - taskPath.currentSite().GetStep();

                    flag = thresholdAlgorithm.solve(taskPath.currentSite(), selectedTask.getPickup(), step, pickup_threshold);

                    if (flag) {

                        step = taskPath.goalSite().GetStep() - pickupSite.GetStep();

                        flag = thresholdAlgorithm.solve(selectedTask.getPickup(), selectedTask.getDelivery(), step, delivery_threshold);

                        if (flag) {

                            std::vector<_site> vsite;

                            token.listChargingEndpoints([&vsite, &token, deliverySite, this](const _site & chargingEndpoint) {

                                this->endpointIndexerAlgorithm.solve(token, chargingEndpoint, deliverySite, vsite);

                                return false;

                            });

                            for (auto endpoint : vsite) {

                                flag = true;

                                if (flag) {

                                    token.listAgents([endpoint, agent, &flag](_agent & otherAgent) {

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

                                        flag = agent.isAbleToFulfillChargingTaskPath(token.getMap(), selectedTask, path);

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

                }

            } 

        }

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

                token.listAgents([endpoint, agent, &aux](_agent & otherAgent) {

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