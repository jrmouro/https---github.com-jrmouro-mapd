#include "_closerTaskIndexerAlgorithm.h"
#include "_token.h"

_taskIndexerAlgorithm* _closerTaskIndexerAlgorithm::getInstance() const{
    return new _closerTaskIndexerAlgorithm();
}

void _closerTaskIndexerAlgorithm::solve(
        const _token& token,
        const _task& task,
        const _site& endPointReference,
        std::vector<_task>& taskIndex) const {

    const unsigned taskDistance = token.getEndpointsDistanceAlgorithm().solve(endPointReference, task.getPickup());

    std::vector<_task>::iterator it = taskIndex.begin();

    for (; it != taskIndex.end(); it++) {

        const unsigned dist = token.getEndpointsDistanceAlgorithm().solve(endPointReference, it->getPickup());

        if (taskDistance < dist) {

            it = taskIndex.insert(it, task);

            return;

        }

    }

    taskIndex.push_back(task);

}