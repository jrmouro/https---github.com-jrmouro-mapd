#include "_selectChargingEndpointToAgentAlgorithm.h"
#include "_taskPathToAgentAlgorithm.h"

bool _selectChargingEndpointToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _site& selectedNewSite,
        _stepPath& selectedPath) const {

    std::vector<_site> vsite;

    token.listChargingEndpoints([&vsite, &token, agent, this](const _site & endpoint) {

        this->endpointIndexerAlgorithm.solve(token, endpoint, agent, vsite);

        return false;

    });

    for (auto endpoint : vsite) {

        bool flag = true;

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
            
            _stepPath chargingPath(selectedPath);

            flag = pathToAgentAlgorithm.solve(token, agent, selectedNewSite, chargingPath);

            if (flag) {

                flag = agent.isAbleToFulfillNoCarryngPath(token.getMap(), chargingPath);

                if (flag) {
                    
                    chargingPath.pop();
                    selectedPath.progress(chargingPath);

                    return true;

                }

            } 

        }

    }

    return false;

}

