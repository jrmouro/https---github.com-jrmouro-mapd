#include "_selectTaskToAgentThresholdAlgorithm.h"
#include "_token.h"
#include "_thresholdAlgorithm.h"

#include "_taskPathToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"

_selectTaskToAgentThresholdAlgorithm::_selectTaskToAgentThresholdAlgorithm(
        const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
        _taskIndexerAlgorithm& taskIndexerAlgorithm,
        float pickup_threshold,
        float delivery_threshold) :
_selectTaskToAgentAlgorithm(
taskPathToAgentAlgorithm,
taskIndexerAlgorithm),
pickup_threshold(pickup_threshold),
delivery_threshold(delivery_threshold) {
}

_selectTaskToAgentThresholdAlgorithm::_selectTaskToAgentThresholdAlgorithm(
        const _selectTaskToAgentThresholdAlgorithm& other) :
_selectTaskToAgentAlgorithm(other),
pickup_threshold(other.pickup_threshold),
delivery_threshold(other.delivery_threshold) {
}

void _selectTaskToAgentThresholdAlgorithm::setDelivery_threshold(float delivery_threshold) {
    this->delivery_threshold = delivery_threshold;
}

void _selectTaskToAgentThresholdAlgorithm::setPickup_threshold(float pickup_threshold) {
    this->pickup_threshold = pickup_threshold;
}

bool _selectTaskToAgentThresholdAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _task& selectedTask,
        _stepPath& selectedPath) const {

    _thresholdAlgorithm thresholdAlgorithm(token.getEndpointsPathAlgorithm());

    std::vector<_task> vtask;

    token.listPendingTasks([&vtask, &token, agent, this](const _task & task) {

        this->taskIndexerAlgorithm.solve(token, task, agent, vtask);

        return false;

    });
    
    bool aux = false;
    _task auxTask;
    _stepPath auxPath;

    for (auto task : vtask) {

        bool flag = true;

        token.listConstAgents([&task, &flag, &agent, this](const _agent& otherAgent) {

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
            
            _stepPath taskPath(selectedPath);

            _stepSite pickupSite, deliverySite;

            flag = taskPathToAgentAlgorithm.solve(token, agent, selectedTask, taskPath, pickupSite, deliverySite);

            if (flag) {
                
                //Guarda para uso futuro
                aux = true;
                auxTask = task;
                auxPath = taskPath;
                
                unsigned step = pickupSite.GetStep() - taskPath.currentSite().GetStep();

                flag = thresholdAlgorithm.solve(taskPath.currentSite(), selectedTask.getPickup(), step, pickup_threshold);

                if (flag) {

                    step = taskPath.goalSite().GetStep() - pickupSite.GetStep();

                    flag = thresholdAlgorithm.solve(selectedTask.getPickup(), selectedTask.getDelivery(), step, delivery_threshold);

                    if (flag) {

                        flag = agent.isAbleToFulfillTaskPath(token.getMap(), selectedTask, taskPath);

                        if (flag) {
                            
                            taskPath.pop();
                            selectedPath.progress(taskPath);

                            return true;

                        }

                    }

                }

            }

        }

    }
    
    if(aux){
        
        aux = agent.isAbleToFulfillTaskPath(token.getMap(), auxTask, auxPath);

        if (aux) {
            
            selectedTask = auxTask;

            auxPath.pop();
            selectedPath.progress(auxPath);

            return true;

        }
        
    }

    return false;

}