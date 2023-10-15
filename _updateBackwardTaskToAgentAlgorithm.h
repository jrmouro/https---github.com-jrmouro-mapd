/* 
 * File:   _updateBackwardTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 18:40
 */

#ifndef _UPDATEBACKWARDTASKTOAGENTALGORITHM_H
#define _UPDATEBACKWARDTASKTOAGENTALGORITHM_H

#include "_updateToAgentAlgorithm.h"

class _token;
class _agent;
class _selectBackwardTaskToAgentAlgorithm;
class _closerCooperatorAgentIndexerAlgorithm;
class _taskIndexerAlgorithm;
class _updateBackwardTaskToAgentAlgorithm  : public _updateToAgentAlgorithm{
    
public:
    
    _updateBackwardTaskToAgentAlgorithm(
            _selectBackwardTaskToAgentAlgorithm&//,
//            _closerCooperatorAgentIndexerAlgorithm&
            );                
    
    _updateBackwardTaskToAgentAlgorithm(const _updateBackwardTaskToAgentAlgorithm& orig);
    
    virtual ~_updateBackwardTaskToAgentAlgorithm(){}
    
    virtual bool solve(_token&, _agent&) const;
    
    void setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm);
    
private:
    
    _selectBackwardTaskToAgentAlgorithm& selectBackwardTaskToAgentAlgorithm;
//    _closerCooperatorAgentIndexerAlgorithm& closerCooperatorAgentIndexerAlgorithm;

};

#endif /* _UPDATEBACKWARDTASKTOAGENTALGORITHM_H */

