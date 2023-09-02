/* 
 * File:   C_TaskCarryThresholdToken.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 09:06
 */

#ifndef C_TASKCARRYTHRESHOLDTOKEN_H
#define C_TASKCARRYTHRESHOLDTOKEN_H

#include "_token.h"

class C_TaskCarryThresholdToken : public _token{
public:
    
    C_TaskCarryThresholdToken(const _map& map, 
            _stepMap& stepMap, 
            std::vector<_site>& endpoints,  
            std::vector<_site>& chargingEndpoints, 
            const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm, 
            float task_threshold, 
            float carry_threshold) :
    _token(map, 
            stepMap, 
            endpoints, 
            chargingEndpoints, 
            endpointsDistanceAlgorithm, 
            task_threshold, 
            carry_threshold) {
    }
    
    C_TaskCarryThresholdToken(const C_TaskCarryThresholdToken& other) : _token(other) { }

    virtual ~C_TaskCarryThresholdToken(){}
    
    virtual std::string id() const {
        
        return "c_tcttp";
        
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent, bool energyCheck){
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInFinishedPath()){

            if(updateTaskOrCtaskPathToAgent(agent, energyCheck)) {

                ret = TokenUpdateType::task;

            } else {

                if(updateRestPathToAgent(agent, energyCheck)){

                    ret = TokenUpdateType::rest;

                } else {

                    ret = this->updateTrivialPathToAgent(agent, energyCheck);

                }

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "agent not parked: " << std::endl << agent << std::endl<< *this;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }
    
        return ret;

    }

};

#endif /* C_TASKCARRYTHRESHOLDTOKEN_H */

