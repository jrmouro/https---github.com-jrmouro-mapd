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
    
    TaskCarryThresholdToken(const TaskCarryThresholdToken& other) : _token(other) { }

    virtual ~TaskCarryThresholdToken(){}
    
    virtual void updatePath(_agent& agent){
    
        if(agent.isParked()){

            if(!updateTaskPathToAgentTaskCarryThreshold(agent))
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

#endif /* TASKCARRYTHRESHOLDTOKEN_H */

