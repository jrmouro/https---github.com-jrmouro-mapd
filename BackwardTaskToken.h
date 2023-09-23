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
    
    BackwardTaskToken(
            const _map& map, 
            const _stepMap& stepMap, 
            const _agent_energy_system& agent_energy_system,
            float delivery_threshold) :
                _token(map, stepMap, agent_energy_system),
                delivery_threshold(delivery_threshold) { }
    
//    BackwardTaskToken(const BackwardTaskToken& other) : 
//                _token(other), 
//                delivery_threshold(other.delivery_threshold){ }

    virtual ~BackwardTaskToken(){}
    
//    virtual _token* builderToken(
//            const _map& map, 
//            const _stepMap& stepMap, 
//            const _agent_energy_system& agent_energy_system,
//            float pickup_threshold = 0, 
//            float delivery_threshold = 0) const{
//        
//        
//        return new BackwardTaskToken(map, stepMap, agent_energy_system, delivery_threshold);
//        
//    }
    
//    virtual _token* getInstance() const {
//        return new BackwardTaskToken(*this);
//    }
    
    virtual std::string name() const {
        
        std::stringstream s;
        s << "BTT(" << delivery_threshold << ")"; 
        
        return s.str();
        
    }
    
    virtual std::string id() const {
        
        return "BTT";
        
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent){
        
//        _closerTaskIndexerThresholdAlgorithm closerTaskIndexerThresholdAlgorithm(this->getMap().getNum_bots());
        _closerTaskIndexerAlgorithm closerTaskIndexerAlgorithm;
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerAlgorithm, .0f, delivery_threshold);
                        
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
        _closerTaskIndexerAlgorithm closerTaskIndexerAlgorithm;
        
        auto uta = _updateTokenAlgorithms::getInstance(closerTaskIndexerAlgorithm, .0f, delivery_threshold);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateBackwardChargingTaskToAgentAlgorithm().solve(*this, agent)) {

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
    
private:
    
    float delivery_threshold;

};

#endif /* BACKWARDTASKTOKEN_H */

