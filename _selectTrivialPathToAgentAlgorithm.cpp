/* 
 * File:   _selectTrivialPathToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 16:48
 */

#include "_selectTrivialPathToAgentAlgorithm.h"
#include "_trivialPathAlgorithm.h"
#include "_token.h"

bool _selectTrivialPathToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _stepPath& trivialPath) const {

    if (agent.isInFinishedPath()) {

        _trivialPathAlgorithm trivialPathAlgorithm;

        trivialPathAlgorithm.solve(agent.currentSite(), trivialPath);
        
        return agent.isAbleToFulfillNoCarryngPath(token.getMap(), trivialPath);
        
    }

    try {
        std::ostringstream stream;
        stream << "agent is not in finished path " << std::endl << agent;
        MAPD_EXCEPTION(stream.str());
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::abort();
    }
    
    return false;

}