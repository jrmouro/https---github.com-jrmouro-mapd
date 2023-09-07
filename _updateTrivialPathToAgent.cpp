/* 
 * File:   _updateTrivialPathToAgent.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 16:29
 */

#include "_updateTrivialPathToAgent.h"
#include "_token.h"

bool _updateTrivialPathToAgent::solve(
        _token& token,
        _agent& agent) const {
    
    if(agent.isInFinishedPath()){

        _stepPath trivialPath;

        bool flag = selectTrivialPathToAgentAlgorithm.solve(token, agent, trivialPath);

        if(flag){

            agent.setPath(trivialPath);

            token.setMoving(agent, trivialPath);

            return true;

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