/* 
 * File:   _updateTaskSwapToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 2 de outubro de 2023, 08:13
 */

#include "_updateTaskSwapToAgentAlgorithm.h"
#include "_selectTaskSwapToAgentAlgorithm.h"
#include "ReportTask.h"
#include "_token.h"
#include "_taskIndexerAlgorithm.h"
#include "_selectTaskToAgentAlgorithm.h"
#include "_swapInfo.h"


_updateTaskSwapToAgentAlgorithm::_updateTaskSwapToAgentAlgorithm(
    _selectTaskSwapToAgentAlgorithm& selectTaskSwapToAgentAlgorithm) :
    selectTaskSwapToAgentAlgorithm(selectTaskSwapToAgentAlgorithm) { }
    
_updateTaskSwapToAgentAlgorithm::_updateTaskSwapToAgentAlgorithm(
    const _updateTaskSwapToAgentAlgorithm& other) :
    selectTaskSwapToAgentAlgorithm(other.selectTaskSwapToAgentAlgorithm) { }

   
_updateTaskSwapToAgentAlgorithm::~_updateTaskSwapToAgentAlgorithm(){}
    
void _updateTaskSwapToAgentAlgorithm::setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm){
    selectTaskSwapToAgentAlgorithm.setTaskIndexerAlgorithm(taskIndexerAlgorithm);
}

void _updateTaskSwapToAgentAlgorithm::setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm){
    selectTaskSwapToAgentAlgorithm.setEndpointIndexerAlgorithm(endpointIndexerAlgorithm);
}

bool _updateTaskSwapToAgentAlgorithm::solve(_token& token, _agent& agent) const {
    
    if (agent.isInGoalSite()) {

        _task task;
        _stepPath taskPath(agent.currentSite());
        std::vector<_swapInfo> swapInfos;

        bool flag = selectTaskSwapToAgentAlgorithm.solve(
                token,
                agent,
                task,
                taskPath, 
                swapInfos);

        if (flag) {    
            
            for (auto si : swapInfos) {
                
                token.unassignTaskSwap(si.GetTaskId(), si.GetAgentId());
                
            }
            
            token.assignTask(task.id(), agent.id());
            token.reportTaskUpdate(agent, task, ReportTask::PathType::task, taskPath);

            agent.assignTask(task, taskPath);
            token.setMoving(agent, taskPath);
            
            
            for (auto si : swapInfos) {
                
                if (!si.GetPath().isTrivial()){
                    
//                    token.unassignTaskSwap(si.GetTaskId(), si.GetAgentId());
                    
                    if(si.GetSwapTaskId() > INT_MIN){                                        
                    
                        token.assignTaskSwap(si.GetSwapTaskId(), si.GetAgentId(), si.GetPath());

//                        if (agent.id() == 1) {;
//                            int i = 0;
//                            for (; i < 20; i++)
//                                token.getStepMap().stepView(token.getCurrentStep() + i);        
//                            token.getStepMap().stepView(token.getCurrentStep() + i);
//                        }
                    
                    } else {
                        
                        token.setMovingSwap(si.GetAgentId(), si.GetPath());
                        
                    }
                    
                    
                } else {
                    
                    try {
                        std::ostringstream stream;
                        stream << "problema aqui!!!!" << std::endl;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }                    
                    
                }                

            }
            

            if (agent.isInGoalSite()) {

                token.runTask(task);
                token.finishTask(task);

                return false;

            }
            
//            if (agent.id() == 1) {;
//                int i = 0;
//                for (; i < 20; i++)
//                    token.getStepMap().stepView(token.getCurrentStep() + i);        
//                token.getStepMap().stepView(token.getCurrentStep() + i);
//            }

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
