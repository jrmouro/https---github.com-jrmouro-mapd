/* 
 * File:   _updateEndpointToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 18:52
 */

#ifndef _UPDATEENDPOINTTOAGENTALGORITHM_H
#define _UPDATEENDPOINTTOAGENTALGORITHM_H


#include "_updateToAgentAlgorithm.h"
#include "_selectEndpointToAgentAlgorithm.h"

class _selectEndpointToAgentAlgorithm;
class _updateEndpointToAgentAlgorithm  : public _updateToAgentAlgorithm{
    
public:
    
    _updateEndpointToAgentAlgorithm( 
            _selectEndpointToAgentAlgorithm& selectEndpointToAgentAlgorithm):
                selectEndpointToAgentAlgorithm(selectEndpointToAgentAlgorithm){}
    
    _updateEndpointToAgentAlgorithm(const _updateEndpointToAgentAlgorithm& orig):
    selectEndpointToAgentAlgorithm(orig.selectEndpointToAgentAlgorithm){}
    
    virtual ~_updateEndpointToAgentAlgorithm(){}
    
    virtual bool solve(
            _token& token, 
            _agent& agent) const;
    
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm);
    
private:
    
    _selectEndpointToAgentAlgorithm& selectEndpointToAgentAlgorithm;
    
};

#endif /* _UPDATEENDPOINTTOAGENTALGORITHM_H */

