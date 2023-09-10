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
    
    virtual _token* getInstance() const {
        return new CarryThresholdToken(*this);
    }
    
    virtual std::string id() const {
        
        return "cttp";
        
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent, bool energyCheck){
        
        TokenUpdateType ret = TokenUpdateType::none;
    
//        if(agent.isInFinishedPath()){
//
//            if(updateTaskPathToAgentCarryThreshold(agent, energyCheck)) {
//
//                ret = TokenUpdateType::task;
//
//            } else {
//
//                if(updateRestPathToAgent(agent, energyCheck)){
//
//                    ret = TokenUpdateType::rest;
//
//                } else {
//
//                    ret = this->updateTrivialPathToAgent(agent, energyCheck);
//                    
//                }
//
//            }
//
//
//        } else {
//
//            try {
//                std::ostringstream stream;
//                stream << "agent not parked: " << std::endl << agent << std::endl<< *this;
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//        }
    
        return ret;
        
    }
    
    virtual _token::TokenUpdateType updateChargingPath(_agent& agent, bool energyCheck){
        
        TokenUpdateType ret = TokenUpdateType::none;
    
//        if(agent.isInFinishedPath()){
//
//            if(updateTaskPathToAgentCarryThreshold(agent, energyCheck)) {
//
//                ret = TokenUpdateType::task;
//
//            } else {
//
//                if(updateRestPathToAgent(agent, energyCheck)){
//
//                    ret = TokenUpdateType::rest;
//
//                } else {
//
//                    ret = this->updateTrivialPathToAgent(agent, energyCheck);
//                    
//                }
//
//            }
//
//
//        } else {
//
//            try {
//                std::ostringstream stream;
//                stream << "agent not parked: " << std::endl << agent << std::endl<< *this;
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//        }
    
        return ret;
        
    }

};

#endif /* CARRYTHRESHOLDTOKEN_H */

