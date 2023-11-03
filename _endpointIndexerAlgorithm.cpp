#include "_endpointIndexerAlgorithm.h"
#include "_token.h"
#include "_ga_token.h"

void _endpointIndexerAlgorithm::solve(
        const _token& token,
        const _site& site,
        const _agent& agent,
        std::vector<_site>& siteIndex) const {

    solve(token, site, agent.currentSite(), siteIndex);

}

void _endpointIndexerAlgorithm::ga_solve_to_agent(
        const _ga_token& token,
        const _site& site,
        const _ga_agent& agent,
        std::vector<_site>& siteIndex) const {

    ga_solve_to_site(token, site, agent.currentSite(), siteIndex);

}
