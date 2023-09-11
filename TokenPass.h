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
    TokenPass(const _map& map, 
            _stepMap& stepMap, 
            std::vector<_site>& endpoints,  
            std::vector<_site>& chargingEndpoints, 
            const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm) :
    _token(map, 
            stepMap, 
            endpoints, 
            chargingEndpoints, 
            endpointsDistanceAlgorithm) {
    }
    
    TokenPass(const TokenPass& other) : _token(other) { }

    virtual ~TokenPass(){}
    
    virtual std::string id() const {
        
        return "TokenPass";
        
    }
    
    virtual _token* getInstance() const {
        return new TokenPass(*this);
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent, bool energyCheck){
        
        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNum_bots());
        
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerThresholdAlgorithm);
                        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInFinishedPath()){

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
    
    virtual _token::TokenUpdateType updateChargingPath(_agent& agent, bool energyCheck){
        
        auto uta = _updateTokenAlgorithms::getInstance();
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInFinishedPath()){

            if(uta->getUpdateChargingTaskToAgentAlgorithm().solve(*this, agent)) {

                ret = TokenUpdateType::charging_task;

            } else {
                
//                if(agent.id()==3){
//                    for (int i = 0; i < 20; i++){
//                        this->getStepMap().stepView(this->getCurrentStep() + i);
//                        
//                    }
//                    
//                }

                if(uta->getUpdateChargingEndpointToAgentAlgorithm().solve(*this, agent)){

                    ret = TokenUpdateType::charging_rest;

                } else {

                    if(uta->getUpdateTrivialPathToAgentAlgorithm().solve(*this, agent)){
                        
                        ret =  TokenUpdateType::charging_trivial;                      
                        
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

