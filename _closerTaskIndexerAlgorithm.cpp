#include "_closerTaskIndexerAlgorithm.h"
#include "_token.h"
#include "_ga_token.h"

_taskIndexerAlgorithm* _closerTaskIndexerAlgorithm::getInstance() const{
    return new _closerTaskIndexerAlgorithm();
}

void _closerTaskIndexerAlgorithm::solve(
        const _token& token,
        const _task& task,
        const _site& endPointReference,
        std::vector<_task>& taskIndex) const {

    const unsigned taskDistance = token.getEndpointsPathAlgorithm()->solve_distance(endPointReference, task.getPickup());

    std::vector<_task>::iterator it = taskIndex.begin();

    for (; it != taskIndex.end(); it++) {

        const unsigned dist = token.getEndpointsPathAlgorithm()->solve_distance(endPointReference, it->getPickup());

        if (taskDistance < dist) {

            taskIndex.insert(it, task);

            return;

        }

    }

    taskIndex.push_back(task);

}

void _closerTaskIndexerAlgorithm::ga_solve(
        const _ga_token& token,
        const _task& task,
        const _site& endPointReference,
        std::vector<_task>& taskIndex) const {

    const unsigned taskDistance = token.getMap().getEndpointsPathAlgorithm()->solve_distance(endPointReference, task.getPickup());

    std::vector<_task>::iterator it = taskIndex.begin();

    for (; it != taskIndex.end(); it++) {

        const unsigned dist = token.getMap().getEndpointsPathAlgorithm()->solve_distance(endPointReference, it->getPickup());

        if (taskDistance < dist) {

            taskIndex.insert(it, task);

            return;

        }

    }

    taskIndex.push_back(task);

}