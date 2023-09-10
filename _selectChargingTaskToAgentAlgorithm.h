/* 
 * File:   _chargingTaskPathToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 19:03
 */

#ifndef _CHARGINGTASKPATHTOAGENTALGORITHM_H
#define _CHARGINGTASKPATHTOAGENTALGORITHM_H

#include "_taskPathToAgentAlgorithm.h"
#include "_endpointIndexerAlgorithm.h"

class _chargingTaskPathToAgentAlgorithm : public _taskPathToAgentAlgorithm {
public:

    _chargingTaskPathToAgentAlgorithm(
            const _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
            const _stepPathAlgorithm& stepPathAlgorithm) :
    _taskPathToAgentAlgorithm(stepPathAlgorithm),
    endpointIndexerAlgorithm(endpointIndexerAlgorithm) { }

    _chargingTaskPathToAgentAlgorithm(const _chargingTaskPathToAgentAlgorithm& other) :
        _taskPathToAgentAlgorithm(other),
        endpointIndexerAlgorithm(other.endpointIndexerAlgorithm) { }

    virtual ~_chargingTaskPathToAgentAlgorithm() { }

    virtual bool solve(
            const _token& token,
            const _agent& agent,
            const _task& task,
            _stepPath& path,
            _stepSite& pickupSite,
            _stepSite& deliverySite) const {

        bool flag = _taskPathToAgentAlgorithm::solve(token, agent, task, path, pickupSite, deliverySite);

        if (flag) {

            std::vector<_site> vsite;

            token.listChargingEndpoints([&vsite, path](const _site & chargingEndpoint) {

                endpointIndexerAlgorithm.solve(chargingEndpoint, path.goalSite(), vsite);

                return false;

            });

            for (auto endpoint : vsite) {

                bool flag = true;

                token.listPendingTasks([endpoint, &flag](const _task & task) {

                    if (task.getDelivery().match(endpoint)) {

                        flag = false;

                        return true;

                    }

                    return false;

                });

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

                    flag = stepPathAlgorithm.solve(token, agent, path, endpoint);

                    if (flag) {

                        return true;

                    } else {

                        try {
                            std::ostringstream stream;
                            stream << "unsolved charging endpoint path: " << endpoint;
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }

                    }

                }

            }

        }

        return false;
        
    }

private:

    const _endpointIndexerAlgorithm& endpointIndexerAlgorithm;

};

#endif /* _CHARGINGTASKPATHTOAGENTALGORITHM_H */

