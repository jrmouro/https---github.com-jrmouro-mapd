/* 
 * File:   TaskSwapTokenPass.h
 * Author: ronaldo
 *
 * Created on 3 de outubro de 2023, 09:06
 */

#ifndef TASKSWAPTOKENPASS_H
#define TASKSWAPTOKENPASS_H

#include "TokenPass.h"
#include "_updateTokenAlgorithms.h"
#include "_updateTaskToAgentAlgorithm.h"
#include "_updateEndpointToAgentAlgorithm.h"
#include "_closerTaskIndexerThresholdAlgorithm.h"

class TaskSwapTokenPass : public TokenPass{
    
public:
    
    TaskSwapTokenPass(
            const _map& map, 
            const _stepMap& stepMap,
            const _agent_energy_system& agent_energy_system,
            unsigned task_threshold) : TokenPass(map, stepMap, agent_energy_system, task_threshold) { }
    
    TaskSwapTokenPass(const TaskSwapTokenPass& other) : TokenPass(other) { }

    virtual ~TaskSwapTokenPass(){}
            
    virtual std::string id() const {
        
        return "TSTP";
        
    }
    
    virtual std::string name() const {
        
        std::stringstream s;
        s << "TSTP[" << _token::name() << "]"; 
        
        return s.str();
        
    }
    
    virtual _token* getClone() const {
        return new TaskSwapTokenPass(*this);
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent){
                
        auto uta = _updateTokenAlgorithms::getInstance(task_threshold);
                        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateTaskSwapToAgentAlgorithm().solve(*this, agent)) {

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
                
        auto uta = _updateTokenAlgorithms::getInstance(task_threshold);
        
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInGoalSite()){

            if(uta->getUpdateTaskSwapToAgentAlgorithm().solve(*this, agent)) {

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

#endif /* TASKSWAPTOKENPASS_H */

