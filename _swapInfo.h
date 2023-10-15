/* 
 * File:   _swapInfo.h
 * Author: ronaldo
 *
 * Created on 27 de setembro de 2023, 14:57
 */

#ifndef _SWAPINFO_H
#define _SWAPINFO_H

#include <climits>

class _swapInfo {
    
public:
    
    _swapInfo(int taskId, int agentId, _stepPath path) :
    _swapInfo(taskId, agentId, path, INT_MIN) { }
    
    _swapInfo(int taskId, int agentId, _stepPath path, int swapTaskId) :
    agentId(agentId), taskId(taskId), swapTaskId(swapTaskId), path(path) { }
    
    _swapInfo(const _swapInfo& other) :
    agentId(other.agentId), taskId(other.taskId), path(other.path), swapTaskId(other.swapTaskId) { }

    virtual ~_swapInfo(){}
    
    const _stepPath& GetPath() const {
        return path;
    }
    
    int GetAgentId() const {
        return agentId;
    }
    
    int GetTaskId() const {
        return taskId;
    } 
    
    int GetSwapTaskId() const {
        return swapTaskId;
    }

    
private:
    
    int agentId, taskId, swapTaskId = INT_MIN;
    _stepPath path;
    
};

#endif /* _SWAPINFO_H */

