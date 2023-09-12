#include "_closerEndpointIndexerAlgorithm.h"

void _closerEndpointIndexerAlgorithm::solve(
        const _token& token,
        const _site& endpoint,
        const _site& endpointReference,
        std::vector<_site>& siteIndex) const {

    const unsigned siteDistance = token.getEndpointsDistanceAlgorithm().solve(endpointReference, endpoint);

    std::vector<_site>::iterator it = siteIndex.begin();

    for (; it != siteIndex.end(); it++) {

        const unsigned dist = token.getEndpointsDistanceAlgorithm().solve(endpointReference, *it);

        if (siteDistance < dist) {

            siteIndex.insert(it, endpoint);

            return;

        }

    }

    siteIndex.push_back(endpoint);

}