/* 
 * File:   _updateTaskToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 6 de setembro de 2023, 18:40
 */

#include "_updateTaskToAgentAlgorithm.h"
#include "ReportTask.h"
#include "_token.h"
#include "_taskIndexerAlgorithm.h"
#include "_selectTaskToAgentAlgorithm.h"

_updateTaskToAgentAlgorithm::_updateTaskToAgentAlgorithm(
        _selectTaskToAgentAlgorithm& selectTaskToAgentAlgorithm) :
                selectTaskToAgentAlgorithm(selectTaskToAgentAlgorithm) {}

_updateTaskToAgentAlgorithm::_updateTaskToAgentAlgorithm(
        const _updateTaskToAgentAlgorithm& orig) :
                selectTaskToAgentAlgorithm(orig.selectTaskToAgentAlgorithm) {}

void _updateTaskToAgentAlgorithm::setTaskIndexerAlgorithm(
        _taskIndexerAlgorithm& taskIndexerAlgorithm) {
    
    selectTaskToAgentAlgorithm.setTaskIndexerAlgorithm(taskIndexerAlgorithm);
    
}

bool _updateTaskToAgentAlgorithm::solve(
        _token& token,
        _agent& agent) const {

    if (agent.isInGoalSite()) {

        _task task;
        _stepPath taskPath(agent.currentSite());

        bool flag = selectTaskToAgentAlgorithm.solve(
                token,
                agent,
                task,
                taskPath);

        if (flag) {

            token.assignTask(task, agent);
            token.reportTaskUpdate(agent, task, ReportTask::PathType::task, taskPath);

            agent.assignTask(task, taskPath);
            token.setMoving(agent, taskPath);

            if (agent.isInGoalSite()) {

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