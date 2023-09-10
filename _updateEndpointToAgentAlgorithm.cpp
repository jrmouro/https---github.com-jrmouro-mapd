/* 
 * File:   _updateRestEndpointToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 18:52
 */

#include "_updateRestEndpointToAgentAlgorithm.h"

bool _updateCloserRestEndpointToAgentAlgorithm::solve(
        _token& token,
        _agent& agent) const {
    
    if(agent.isInFinishedPath()){

            _stepPath restPath; 
            _site restEndpoint; 
            _task conflitTask;

        bool flag = selectRestEndpointToAgentAlgorithm.solve(
                token, 
                agent, 
                restEndpoint,
                restPath,
                conflitTask);

        if(flag){
            
            token.reportTaskUpdate(agent, conflitTask, ReportTask::PathType::rest, restPath);  

            agent.setPath(restPath);

            token.setMoving(agent, restPath);

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

