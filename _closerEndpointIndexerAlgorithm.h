/* 
 * File:   _closerEndpointIndexerAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 17:10
 */

#ifndef _CLOSERENDPOINTINDEXERALGORITHM_H
#define _CLOSERENDPOINTINDEXERALGORITHM_H

#include "_endpointIndexerAlgorithm.h"
#include "_token.h"

class _token;
class _closerEndpointIndexerAlgorithm : public _endpointIndexerAlgorithm{
public:
    
    _closerEndpointIndexerAlgorithm() { }
    
    _closerEndpointIndexerAlgorithm(const _closerEndpointIndexerAlgorithm& other){ }
    
    virtual _endpointIndexerAlgorithm* getInstance() const;
    
    virtual void solve(
            const _token& token,
            const _site& endpoint, 
            const _site& endpointReference, 
            std::vector<_site>& siteIndex) const;
    
};

#endif /* _CLOSERENDPOINTINDEXERALGORITHM_H */

