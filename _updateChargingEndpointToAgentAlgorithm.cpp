/* 
 * File:   _updateCloserChargingEndpointToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 19:05
 */

#include "_updateChargingEndpointToAgentAlgorithm.h"

bool _updateChargingEndpointToAgentAlgorithm::solve(
        _token& token,
        _agent& agent) const {

    if(agent.isInFinishedPath()){

            _stepPath chargingPath; 
            _site chargingEndpoint; 

        bool flag = selectChargingEndpointToAgentAlgorithm.solve(
                token, 
                agent, 
                chargingEndpoint,
                chargingPath);

        if(flag){
            
            if (!chargingPath.isTrivial()){
        
                agent.setPath(chargingPath);

                token.setMoving(agent, chargingPath);

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