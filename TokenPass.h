/* 
 * File:   TokenPass.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 09:06
 */

#ifndef TOKENPASS_H
#define TOKENPASS_H

#include "_token.h"
#include "_updateTokenAlgorithms.h"
#include "_updateTaskToAgentAlgorithm.h"
#include "_updateEndpointToAgentAlgorithm.h"
#include "_closerTaskIndexerThresholdAlgorithm.h"

class TokenPass : public _token{
    
public:
    
    TokenPass(
            const _map& map, 
            const _stepMap& stepMap,
            const _agent_energy_system& agent_energy_system) : _token(map, stepMap, agent_energy_system) { }
    
//    TokenPass(const TokenPass& other) : _token(other) { }

    virtual ~TokenPass(){}
            
    virtual std::string id() const {
        
        return "TP";
        
    }
    
    virtual std::string name() const {
        
        return "TP";
        
    }
    
//    virtual _token* builderToken(
//            const _map& map, 
//            const _stepMap& stepMap, 
//            const _agent_energy_system& agent_energy_system,
//            float pickup_threshold = 0, 
//            float delivery_threshold = 0) const{
//        
//        
//        return new TokenPass(map, stepMap, agent_energy_system);
//        
//    }
    
//    virtual _token* getInstance() const {
//        return new TokenPass(*this);
//    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent){
        
        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNumBots());
        
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerThresholdAlgorithm);
                        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateTaskToAgentAlgorithm().solve(*this, agent)) {

                ret = TokenUpdateType::task;

            } else {

                if(uta->getUpdateRestEndpointToAgentAlgorithm().solve(*this, agent)){

                    ret = TokenUpdateType::rest;

                } else {

                    if(uta->getUpdateTrivialPathToAgentAlgorithm().solve(*this, agent)){
                        
                        ret =  TokenUpdateType::trivial;                      
                        
                    } else {
                        
                        try {
                            std::ostringstream stream;
                            stream << "update trivial path error:" << std::endl << agent << std::endl<< *this;
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }
                        
                    }

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
    
    virtual _token::TokenUpdateType updateChargingPath(_agent& agent){
        
        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNumBots());
        
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerThresholdAlgorithm);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateChargingTaskToAgentAlgorithm().solve(*this, agent)) {

                ret = TokenUpdateType::charging_task;

            } else {

                if(uta->getUpdateChargingEndpointToAgentAlgorithm().solve(*this, agent)){

                    ret = TokenUpdateType::charging_rest;

                } else {

                    if(uta->getUpdateTrivialPathToAgentAlgorithm().solve(*this, agent)){
                        
                        ret =  TokenUpdateType::trivial;                      
                        
                    } else {
                        
                        try {
                            std::ostringstream stream;
                            stream << "update trivial path error:" << std::endl << agent << std::endl<< *this;
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }
                        
                    }

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

#endif /* TOKENPASS_H */

