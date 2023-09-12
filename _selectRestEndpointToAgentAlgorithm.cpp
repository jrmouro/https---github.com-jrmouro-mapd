#include "_selectRestEndpointToAgentAlgorithm.h"
#include "_token.h"


bool _selectRestEndpointToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _site& selectedNewSite,
        _stepPath& selectedPath) const {

    bool flag = token.isTaskDeliveryEndpoint(agent.currentSite());

    if (flag) {

        std::vector<_site> vsite;

        token.listNonTaskDeliveryEndpoints([&vsite, &token, agent, this](const _site & endpoint) {

            this->endpointIndexerAlgorithm.solve(token, endpoint, agent, vsite);

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

                selectedNewSite = endpoint;
                
                _stepPath restPath(selectedPath);

                flag = pathToAgentAlgorithm.solve(token, agent, selectedNewSite, restPath);

                if(flag){

                    flag = agent.isAbleToFulfillNoCarryngPath(token.getMap(), restPath);

                    if (flag) {
                        
                        restPath.pop();
                        selectedPath.progress(restPath);

                        return true;

                    }

                }

            }

        }

    }

    return false;

}

