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
#include "_closerTaskIndexerThresholdAlgorithm.h"

class ThresholdTokenPass : public _token{
public:
    ThresholdTokenPass(
            const _map& map, 
            const _stepMap& stepMap, 
            const _agent_energy_system& agent_energy_system,
            float pickup_threshold, 
            float delivery_threshold)  : 
                _token(map, stepMap, agent_energy_system),
                pickup_threshold(pickup_threshold), 
                delivery_threshold(delivery_threshold) { }
    
//    ThresholdTokenPass(const ThresholdTokenPass& other) : 
//                _token(other),
//                pickup_threshold(other.pickup_threshold), 
//                delivery_threshold(other.delivery_threshold) { }

    virtual ~ThresholdTokenPass(){}
    
//    virtual _token* builderToken(
//            const _map& map, 
//            const _stepMap& stepMap, 
//            const _agent_energy_system& agent_energy_system,
//            float pickup_threshold = 0, 
//            float delivery_threshold = 0) const{
//        
//        
//        return new ThresholdTokenPass(map, stepMap, agent_energy_system, pickup_threshold, delivery_threshold);
//        
//    }
    
//    virtual _token* getInstance() const {
//        return new ThresholdTokenPass(*this);
//    }
    
    virtual std::string name() const {
        
        std::stringstream s;
        s << "TTP(" << pickup_threshold << " : " << delivery_threshold << ")"; 
        
        return s.str();
        
    }
    
    virtual std::string id() const {
        
        return "TTP";
        
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent){
        
//        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNum_bots());
        _closerTaskIndexerAlgorithm closerTaskIndexerAlgorithm;
        
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerAlgorithm, pickup_threshold, delivery_threshold);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

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
    
    virtual _token::TokenUpdateType updateChargingPath(_agent& agent){
        
//        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNum_bots());
        _closerTaskIndexerAlgorithm closerTaskIndexerAlgorithm;
        
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerAlgorithm, pickup_threshold, delivery_threshold);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateChargingTaskToAgentThresholdAlgorithm().solve(*this, agent)) {

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
    
    void setDelivery_threshold(float delivery_threshold) {
        this->delivery_threshold = delivery_threshold;
    }

    void setPickup_threshold(float pickup_threshold) {
        this->pickup_threshold = pickup_threshold;
    }
    
private:
    
    float pickup_threshold, delivery_threshold;

};

#endif /* THRESHOLDTOKENPASS_H */

