#include "_agentsUpdatePath.h"

#include <climits>

#include "_agent_energy_regime.h"
#include "_ga_token.h"
#include "_ga_agent.h"

_agentsUpdatePath::_agentsUpdatePath(const _stepPathAlgorithm& stepPathAlgorithm) :
    stepPathAlgorithm(stepPathAlgorithm) {}

_agentsUpdatePath::_agentsUpdatePath(const _agentsUpdatePath& other) :
    stepPathAlgorithm(other.stepPathAlgorithm) {}

void _agentsUpdatePath::solve(_ga_token& token, _stepMap& map, const std::vector<std::pair<int, int>>& agentsTasksUpdatePlanning) const {
        
    for (auto agentTaskPair : agentsTasksUpdatePlanning) {
                    
        token.updateAgentTaskPath(agentTaskPair.first, agentTaskPair.second, stepPathAlgorithm, map);           

    }   

}