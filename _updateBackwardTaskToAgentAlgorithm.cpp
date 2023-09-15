/* 
 * File:   _updateBackwardTaskToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 18:40
 */

#include "_updateBackwardTaskToAgentAlgorithm.h"
#include "ReportTask.h"
#include "_token.h"
#include "_taskIndexerAlgorithm.h"
#include "_selectTaskToAgentAlgorithm.h"
#include "_selectBackwardTaskToAgentAlgorithm.h"

_updateBackwardTaskToAgentAlgorithm::_updateBackwardTaskToAgentAlgorithm(
        _selectBackwardTaskToAgentAlgorithm& selectBackwardTaskToAgentAlgorithm,
        _closerCooperatorAgentIndexerAlgorithm& closerCooperatorAgentIndexerAlgorithm) :
                selectBackwardTaskToAgentAlgorithm(selectBackwardTaskToAgentAlgorithm),
                closerCooperatorAgentIndexerAlgorithm(closerCooperatorAgentIndexerAlgorithm){}

_updateBackwardTaskToAgentAlgorithm::_updateBackwardTaskToAgentAlgorithm(
        const _updateBackwardTaskToAgentAlgorithm& orig) :
                selectBackwardTaskToAgentAlgorithm(orig.selectBackwardTaskToAgentAlgorithm),
                closerCooperatorAgentIndexerAlgorithm(orig.closerCooperatorAgentIndexerAlgorithm){}

void _updateBackwardTaskToAgentAlgorithm::setTaskIndexerAlgorithm(
        _taskIndexerAlgorithm& taskIndexerAlgorithm) {
    
    selectBackwardTaskToAgentAlgorithm.setTaskIndexerAlgorithm(taskIndexerAlgorithm);
    
}

bool _updateBackwardTaskToAgentAlgorithm::solve(
        _token& token,
        _agent& agent) const {

    if (agent.isInGoalSite()) {

        _task originalTask, selectedTask(token.getOneTaskId()), pendingTask(token.getOneTaskId());
        _stepPath selectedPath(agent.currentSite());

        bool flag = selectBackwardTaskToAgentAlgorithm.solve(
                token,
                agent,
                originalTask,
                selectedTask,
                pendingTask,
                selectedPath);

        if (flag) {
            
            bool orig = originalTask.getDelivery().match(selectedTask.getDelivery());
            
            token.assignTask(originalTask, agent);
                        
            if(orig){
                
                token.reportTaskUpdate(agent, originalTask, ReportTask::PathType::task, selectedPath);
                agent.assignTask(originalTask, selectedPath);  
                
                token.giveBackOneTaskId();
                token.giveBackOneTaskId();
                
            } else {
                
                token.runTask(originalTask);
                token.finishTask(originalTask);
                
                token.addPendingTask(selectedTask);               
                token.assignTask(selectedTask, agent);
                token.reportTaskUpdate(agent, selectedTask, ReportTask::PathType::backward_task, selectedPath);                
                agent.assignTask(selectedTask, selectedPath);
                
                token.addBackwardTask(token.getCurrentStep() + selectedPath.size(), pendingTask);               
                
            }
            
            token.setMoving(agent, selectedPath);

            if (agent.isInGoalSite()) {
                
                auto agentTask = agent.currentTask();

                token.runTask(agentTask);
                token.finishTask(agentTask);

                return false;

            }

            return true;

        } else {
            
            token.giveBackOneTaskId();
            token.giveBackOneTaskId();
            
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