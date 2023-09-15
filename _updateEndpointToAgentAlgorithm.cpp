/* 
 * File:   _updateRestEndpointToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 18:52
 */

#include "_updateEndpointToAgentAlgorithm.h"

void _updateEndpointToAgentAlgorithm::setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm) {
    selectEndpointToAgentAlgorithm.setEndpointIndexerAlgorithm(endpointIndexerAlgorithm);
}

bool _updateEndpointToAgentAlgorithm::solve(
        _token& token,
        _agent& agent) const {

    if (agent.isInGoalSite()) {

        _stepPath restPath(agent.currentSite());
        _site restEndpoint;

        bool flag = selectEndpointToAgentAlgorithm.solve(
                token,
                agent,
                restEndpoint,
                restPath);

        if (flag) {

            if (!restPath.isTrivial()) {

                agent.setPath(restPath);

                token.setMoving(agent, restPath);

                return true;

            }

        }

    } else {

        try {
            std::ostringstream stream;
            stream << "agent is not in finished path " << std::endl << agent;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return false;

}

