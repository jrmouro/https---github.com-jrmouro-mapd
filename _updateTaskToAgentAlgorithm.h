/* 
 * File:   _updateTaskToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de setembro de 2023, 18:40
 */

#ifndef _UPDATETASKTOAGENTALGORITHM_H
#define _UPDATETASKTOAGENTALGORITHM_H


class _token;
class _agent;
class _selectTaskToAgentAlgorithm;
class _taskIndexerAlgorithm;
class _updateTaskToAgentAlgorithm {
    
public:
    
    _updateTaskToAgentAlgorithm(_selectTaskToAgentAlgorithm&);                
    
    _updateTaskToAgentAlgorithm(const _updateTaskToAgentAlgorithm& orig);
    
    virtual ~_updateTaskToAgentAlgorithm(){}
    
    virtual bool solve(_token&, _agent&) const;
    
    void setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm);
    
private:
    
    _selectTaskToAgentAlgorithm& selectTaskToAgentAlgorithm;

};

#endif /* _UPDATETASKTOAGENTALGORITHM_H */

