/* 
 * File:   TaskThresholdToken.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 09:06
 */

#ifndef TASKTHRESHOLDTOKEN_H
#define TASKTHRESHOLDTOKEN_H

#include "_token.h"

class TaskThresholdToken : public _token{
public:
    TaskThresholdToken(const _map& map, 
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
    
    TaskThresholdToken(const TaskThresholdToken& other) : _token(other) { }

    virtual ~TaskThresholdToken(){}
    
    virtual void updatePath(_agent& agent){
    
        if(agent.isParked()){

            if(!updateTaskPathToAgentTaskThreshold(agent))
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

#endif /* TASKTHRESHOLDTOKEN_H */

