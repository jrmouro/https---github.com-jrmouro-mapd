/* 
 * File:   TaskThresholdToken.cpp
 * Author: ronaldo
 * 
 * Created on 23 de agosto de 2023, 09:06
 */

#include "TaskThresholdToken.h"
//
//
//void TaskThresholdToken::updatePath(_agent& agent){
//    
//    if(agent.isParked()){
//
//        if(!updateTaskPathToAgentTaskThreshold(agent))
//            if(!updateRestPathToAgent(agent))
//                this->updateTrivialPathToAgent(agent);
//    
//    } else {
//        
//        try {
//            std::ostringstream stream;
//            stream << "agent not parked: " << std::endl << agent << std::endl<< *this;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//        
//    }
//    
//}