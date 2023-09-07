#include "_stepPathAlgorithm.h"
#include "_token.h"

bool _stepPathAlgorithm::solve(const _token& token, const _agent& agent, _stepPath& path, const _site& goal) const {
    return solve(token.getStepMap(), path, goal, agent.id());
}