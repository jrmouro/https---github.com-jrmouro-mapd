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

            _stepSite pickupSite, deliverySite;

            flag = taskPathToAgentAlgorithm.solve(token, agent, selectedTask, selectedPath, pickupSite, deliverySite);

            if (flag) {

                if (flag) {

                    flag = thresholdAlgorithm.solve(selectedPath, pickupSite.GetStep(), pickup_threshold);

                    if (flag) {

                        unsigned step = selectedPath.goalSite().GetStep() - pickupSite.GetStep();

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

//                                    if (flag) {
//
//                                        flag = token.getStepMap().isPathDefinitelyFree(chargingPath.goalSite());

                                        if (flag) {

                                            chargingPath.pop();
                                            _stepPath path(selectedPath);
                                            path.progress(chargingPath);

                                            flag = agent.isAbleToFulfillChargingTaskPath(token.getMap(), selectedTask, path);

                                            if (flag) {

                                                selectedPath.progress(chargingPath);

                                                return true;

                                            }

                                        }

//                                    } 
                                    
//                                    else {
//
//                                        try {
//                                            std::ostringstream stream;
//                                            stream << "unsolved charging endpoint path: " << endpoint;
//                                            MAPD_EXCEPTION(stream.str());
//                                        } catch (std::exception& e) {
//                                            std::cout << e.what() << std::endl;
//                                            std::abort();
//                                        }
//
//                                    }

                                }

                            }

                        }

                    }

                }

            } 
            
//            else {
//
//                try {
//                    std::ostringstream stream;
//                    stream << "unsolved task path: " << task;
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                }
//
//            }


        }

    }

    return false;

}