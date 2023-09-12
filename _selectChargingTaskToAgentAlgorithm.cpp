#include "_selectChargingTaskToAgentAlgorithm.h"
#include "_token.h"
#include "_taskPathToAgentAlgorithm.h"

bool _selectChargingTaskToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _task& selectedTask,
        _stepPath& selectedPath) const {

    std::vector<_task> vtask;

    token.listPendingTasks([&vtask, &token, agent, this](const _task & task) {

        this->taskIndexerAlgorithm.solve(token, task, agent, vtask);

        return false;

    });

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

    return false;

}