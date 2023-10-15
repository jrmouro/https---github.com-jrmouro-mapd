/* 
 * File:   _updateTaskSwapToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 2 de outubro de 2023, 08:13
 */

#ifndef _UPDATETASKSWAPTOAGENTALGORITHM_H
#define _UPDATETASKSWAPTOAGENTALGORITHM_H

#include "_updateToAgentAlgorithm.h"

class _token;
class _agent;
class _selectTaskSwapToAgentAlgorithm;
class _endpointIndexerAlgorithm;
class _taskIndexerAlgorithm;

class _updateTaskSwapToAgentAlgorithm : public _updateToAgentAlgorithm{
    
public:
    
    _updateTaskSwapToAgentAlgorithm(_selectTaskSwapToAgentAlgorithm& selectTaskSwapToAgentAlgorithm);
    
    _updateTaskSwapToAgentAlgorithm(const _updateTaskSwapToAgentAlgorithm& other);
       
    virtual ~_updateTaskSwapToAgentAlgorithm();
    
    virtual bool solve(_token&, _agent&) const;
    
    void setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm);
    void setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm);
    
private:
    _selectTaskSwapToAgentAlgorithm& selectTaskSwapToAgentAlgorithm;
};

#endif /* _UPDATETASKSWAPTOAGENTALGORITHM_H */

