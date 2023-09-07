#include "_selectChargingEndpointToAgentAlgorithm.h"

bool _selectChargingEndpointToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _site& selectedNewSite,
        _stepPath& selectedPath) const {

    std::vector<_site> vsite;

    token.listChargingEndpoints([&vsite, token, agent, this](const _site & endpoint) {

        this->endpointIndexerAlgorithm.solve(token, endpoint, agent, vsite);

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

            selectedNewSite = endpoint;

            flag = pathToAgentAlgorithm.solve(token, agent, selectedNewSite, selectedPath);

            if (flag) {

                flag = agent.isAbleToFulfillNoCarryngPath(token.getMap(), selectedPath);

                if (flag) {

                    return true;

                }

            } else {

                try {
                    std::ostringstream stream;
                    stream << "unsolved endpoint path: " << endpoint;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        }

    }

    return false;

}

