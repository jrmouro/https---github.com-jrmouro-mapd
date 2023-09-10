/* 
 * File:   ThresholdTokenPass.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 09:06
 */

#ifndef THRESHOLDTOKENPASS_H
#define THRESHOLDTOKENPASS_H

#include <sstream>
#include "_token.h"
#include "_updateTokenAlgorithms.h"

class ThresholdTokenPass : public _token{
public:
    ThresholdTokenPass(
            const _map& map, 
            _stepMap& stepMap, 
            std::vector<_site>& endpoints,  
            std::vector<_site>& chargingEndpoints, 
            const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm, 
            float pickup_threshold, 
            float delivery_threshold) :
                _token(map, 
                        stepMap, 
                        endpoints, 
                        chargingEndpoints, 
                        endpointsDistanceAlgorithm), 
                pickup_threshold(pickup_threshold), 
                delivery_threshold(delivery_threshold){
    }
    
    ThresholdTokenPass(const ThresholdTokenPass& other) : _token(other) { }

    virtual ~ThresholdTokenPass(){}
    
    virtual _token* getInstance() const {
        return new ThresholdTokenPass(*this);
    }
    
    virtual std::string id() const {
        
        std::stringstream s;
        s << "ThresholdTokenPass(" << pickup_threshold << " : " << delivery_threshold << ")"; 
        
        return s.str();
        
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent, bool energyCheck){
        
        auto uta = _updateTokenAlgorithms::getInstance(pickup_threshold, delivery_threshold);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInFinishedPath()){

            if(uta->getUpdateTaskToAgentThresholdAlgorithm().solve(*this, agent)) {

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
        
        auto uta = _updateTokenAlgorithms::getInstance(pickup_threshold, delivery_threshold);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInFinishedPath()){

            if(uta->getUpdateChargingTaskToAgentThresholdAlgorithm().solve(*this, agent)) {

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
    
    float pickup_threshold, delivery_threshold;

};

#endif /* THRESHOLDTOKENPASS_H */

