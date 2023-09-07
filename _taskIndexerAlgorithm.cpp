#include "_taskIndexerAlgorithm.h"
#include "_token.h"

void _taskIndexerAlgorithm::solve(
        const _token& token,
        const _task& task,
        const _agent& agent,
        std::vector<_task>& taskIndex) const {

    solve(token, task, agent.currentSite(), taskIndex);

}