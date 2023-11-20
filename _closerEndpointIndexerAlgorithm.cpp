#include "_closerEndpointIndexerAlgorithm.h"
#include "_ga_token.h"

_endpointIndexerAlgorithm* _closerEndpointIndexerAlgorithm::getInstance() const {
    return new _closerEndpointIndexerAlgorithm();
}

void _closerEndpointIndexerAlgorithm::solve(
        const _token& token,
        const _site& endpoint,
        const _site& endpointReference,
        std::vector<_site>& siteIndex) const {

    const unsigned siteDistance = token.getEndpointsPathAlgorithm().solve_distance(endpointReference, endpoint);

    std::vector<_site>::iterator it = siteIndex.begin();

    for (; it != siteIndex.end(); it++) {

        const unsigned dist = token.getEndpointsPathAlgorithm().solve_distance(endpointReference, *it);

        if (siteDistance < dist) {

            siteIndex.insert(it, endpoint);

            return;

        }

    }

    siteIndex.push_back(endpoint);

}

void _closerEndpointIndexerAlgorithm::ga_solve_to_site(
        const _ga_token& token,
        const _site& endpoint,
        const _site& endpointReference,
        std::vector<_site>& siteIndex) const {
    
    const _endpointsPathAlgorithm& endpointsPathAlgorithm = token.getMap().getEndpointsPathAlgorithm();

    const unsigned siteDistance = endpointsPathAlgorithm.solve_distance(endpointReference, endpoint);

    std::vector<_site>::iterator it = siteIndex.begin();

    for (; it != siteIndex.end(); it++) {

        const unsigned dist = endpointsPathAlgorithm.solve_distance(endpointReference, *it);

        if (siteDistance < dist) {

            siteIndex.insert(it, endpoint);

            return;

        }

    }

    siteIndex.push_back(endpoint);

}