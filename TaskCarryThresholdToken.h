/* 
 * File:   TaskCarryThresholdToken.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 09:06
 */

#ifndef TASKCARRYTHRESHOLDTOKEN_H
#define TASKCARRYTHRESHOLDTOKEN_H

#include "_token.h"

class TaskCarryThresholdToken : public _token{
public:
    TaskCarryThresholdToken(const _map& map, 
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
    
    TaskCarryThresholdToken(const TaskCarryThresholdToken& other) : _token(other) { }

    virtual ~TaskCarryThresholdToken(){}
    
    virtual std::string id() const {
        
        return "tcttp";
        
    }
    
    virtual _token::TokenUpdateType updatePath(_agent& agent, bool energyCheck){
    
        TokenUpdateType ret = TokenUpdateType::none;
    
        if(agent.isInFinishedPath()){

            if(updateTaskPathToAgentTaskCarryThreshold(agent, energyCheck)) {

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

#endif /* TASKCARRYTHRESHOLDTOKEN_H */

