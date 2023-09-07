/* 
 * File:   _updateTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 18:40
 */

#ifndef _UPDATETASKTOAGENTALGORITHM_H
#define _UPDATETASKTOAGENTALGORITHM_H

#include "_selectTaskToAgentAlgorithm.h"

class _token;
class _updateTaskToAgentAlgorithm {
    
public:
    
    _updateTaskToAgentAlgorithm( 
            const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
            const _taskIndexerAlgorithm& taskIndexerAlgorithm):
                selectTaskToAgentAlgorithm(
                    taskPathToAgentAlgorithm,
                    taskIndexerAlgorithm){}
                
    
    _updateTaskToAgentAlgorithm(const _updateTaskToAgentAlgorithm& orig):
        selectTaskToAgentAlgorithm(orig.selectTaskToAgentAlgorithm){ }
    
    virtual ~_updateTaskToAgentAlgorithm(){ }
    
    virtual bool solve(
            _token& token, 
            _agent& agent) const;
    
private:
    
    _selectTaskToAgentAlgorithm selectTaskToAgentAlgorithm;

};

#endif /* _UPDATETASKTOAGENTALGORITHM_H */

