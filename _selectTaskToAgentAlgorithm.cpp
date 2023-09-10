#include "_selectTaskToAgentAlgorithm.h"
#include "_token.h"
#include "_taskPathToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"

_selectTaskToAgentAlgorithm::_selectTaskToAgentAlgorithm(
        const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
        _taskIndexerAlgorithm& taskIndexerAlgorithm) :
taskPathToAgentAlgorithm(taskPathToAgentAlgorithm),
taskIndexerAlgorithm(taskIndexerAlgorithm) {
}

_selectTaskToAgentAlgorithm::_selectTaskToAgentAlgorithm(const _selectTaskToAgentAlgorithm& other) :
taskPathToAgentAlgorithm(other.taskPathToAgentAlgorithm),
taskIndexerAlgorithm(other.taskIndexerAlgorithm) {
}

void _selectTaskToAgentAlgorithm::setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm) {
    this->taskIndexerAlgorithm = taskIndexerAlgorithm;
}

bool _selectTaskToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _task& selectedTask,
        _stepPath& selectedPath) const {

    std::vector<_task> vtask;

    token.listPendingTasks([&vtask, &token, agent, this](const _task & task) {

        this->taskIndexerAlgorithm.solve(token, task, agent, vtask);

        return false;

    });

    for (auto task : vtask) {

        bool flag = true;

        token.listAgents([task, &flag, agent](const _agent & otherAgent) {

            if (otherAgent.id() != agent.id()) {

                if (otherAgent.goalSite().match(task.getPickup()) || otherAgent.goalSite().match(task.getDelivery())) {

                    flag = false;

                    return true;

                }

            }

            return false;

        });

        if (flag) {

            selectedTask = task;

            _stepSite pickupSite, deliverySite;

            flag = taskPathToAgentAlgorithm.solve(token, agent, selectedTask, selectedPath, pickupSite, deliverySite);

            //            if (flag) {
            //                
            //                flag = token.getStepMap().isPathDefinitelyFree(selectedPath.goalSite());

            if (flag) {

                flag = agent.isAbleToFulfillTaskPath(token.getMap(), selectedTask, selectedPath);

                if (flag) {

                    return true;

                }

            }

            //            } 

            //            else {
            //
            //                try {
            //                    std::ostringstream stream;
            //                    stream << "unsolved task path: " << task;
            //                    MAPD_EXCEPTION(stream.str());
            //                } catch (std::exception& e) {
            //                    std::cout << e.what() << std::endl;
            //                    std::abort();
            //                }
            //
            //            }


        }

    }

    return false;

}