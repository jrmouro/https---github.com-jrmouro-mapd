#include "_endpointIndexerAlgorithm.h"
#include "_token.h"

void _endpointIndexerAlgorithm::solve(
        const _token& token,
        const _site& site,
        const _agent& agent,
        std::vector<_site>& siteIndex) const {

    solve(token, site, agent.currentSite(), siteIndex);

}
