/* 
 * File:   _updateCloserTaskToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 18:40
 */

#include "_updateTaskToAgentAlgorithm.h"
#include "ReportTask.h"
#include "_token.h"

bool _updateTaskToAgentAlgorithm::solve(
            _token& token, 
            _agent& agent) const{
    
    if(agent.isInFinishedPath()){

            _task task; 
            _stepPath taskPath; 

        bool flag = selectTaskToAgentAlgorithm.solve(
                token, 
                agent, 
                task,
                taskPath);

        if(flag){

            token.assignTask(task, agent);
            token.reportTaskUpdate(agent, task, ReportTask::PathType::task, taskPath);
            
            agent.assignTask(task, taskPath);
            token.setMoving(agent, taskPath);

            if (agent.isInFinishedPath()) {

                token.runTask(task);
                token.finishTask(task);
                
                return false;

            } 

            return true;

        }
    
    } else {
        
        try {
            std::ostringstream stream;
            stream << "agent is not in finished path " << std::endl << agent;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
    
    }
         
    return false;
    
}