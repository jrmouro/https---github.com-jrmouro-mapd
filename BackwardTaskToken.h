/* 
 * File:   BackwardTaskToken.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 09:06
 */

#ifndef BACKWARDTASKTOKEN_H
#define BACKWARDTASKTOKEN_H

#include "_token.h"

class BackwardTaskToken : public _token{
public:
    BackwardTaskToken(const _map& map, 
            _stepMap& stepMap, 
            std::vector<_site>& endpoints, 
            std::vector<_site>& chargingEndpoints,  
            const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm, 
            float delivery_threshold) :
            _token(map, stepMap, endpoints, chargingEndpoints, endpointsDistanceAlgorithm),
            delivery_threshold(delivery_threshold) {
    }
    
    BackwardTaskToken(const BackwardTaskToken& other) : _token(other) { }

    virtual ~BackwardTaskToken(){}
    
    virtual _token* getInstance() const {
        return new BackwardTaskToken(*this);
    }
    
    virtual std::string id() const {
        
        std::stringstream s;
        s << "BackwardTaskToken(" << delivery_threshold << ")"; 
        
        return s.str();
        
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent){
        
//        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNum_bots());
        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(500);
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerThresholdAlgorithm, .0f, delivery_threshold);
                        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateBackwardTaskToAgentAlgorithm().solve(*this, agent)) {

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
        
//        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNum_bots());
        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(500);
        
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerThresholdAlgorithm, .0f, delivery_threshold);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateBackwardChargingTaskToAgentAlgorithm().solve(*this, agent)) {

                ret = TokenUpdateType::charging_task;

            } else {

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
    
private:
    
    float delivery_threshold;

};

#endif /* BACKWARDTASKTOKEN_H */

