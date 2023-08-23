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
            const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm, 
            float task_threshold, 
            float carry_threshold) :
    _token(map, 
            stepMap, 
            endpoints, 
            endpointsDistanceAlgorithm, 
            task_threshold, 
            carry_threshold) {
    }
    
    CarryThresholdToken(const CarryThresholdToken& other) : _token(other) { }

    virtual ~CarryThresholdToken(){}
    
    virtual void updatePath(_agent& agent){
    
        if(agent.isParked()){

            if(!updateTaskPathToAgentCarryThreshold(agent))
                if(!updateRestPathToAgent(agent))
                    this->updateTrivialPathToAgent(agent);

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

    }

};

#endif /* CARRYTHRESHOLDTOKEN_H */

