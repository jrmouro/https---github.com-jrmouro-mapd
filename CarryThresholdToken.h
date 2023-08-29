/* 
 * File:   CarryThresholdToken.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 09:06
 */

#ifndef CARRYTHRESHOLDTOKEN_H
#define CARRYTHRESHOLDTOKEN_H

#include "_token.h"

class CarryThresholdToken : public _token{
public:
    CarryThresholdToken(const _map& map, 
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
    
    CarryThresholdToken(const CarryThresholdToken& other) : _token(other) { }

    virtual ~CarryThresholdToken(){}
    
    virtual _token::TokenUpdateType updatePath(_agent& agent){
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInFinishedPath()){

            if(updateTaskPathToAgentCarryThreshold(agent)) {

                ret = TokenUpdateType::task;

            } else {

                if(updateRestPathToAgent(agent)){

                    ret = TokenUpdateType::rest;

                } else {

                    this->updateTrivialPathToAgent(agent);

                    ret = TokenUpdateType::trivial;

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

};

#endif /* CARRYTHRESHOLDTOKEN_H */

