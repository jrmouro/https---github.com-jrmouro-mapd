#include "_selectTaskSwapToAgentAlgorithm.h"
#include "_token.h"
#include "_taskPathToAgentAlgorithm.h"
#include "_selectRestEndpointToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"
#include "_swapInfo.h"
#include "_chargingTaskToAgentAlgorithm.h"
#include <climits>

_selectTaskSwapToAgentAlgorithm::_selectTaskSwapToAgentAlgorithm(
        const _pathToAgentAlgorithm& pathToAgentAlgorithm, 
        const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm, 
        _endpointIndexerAlgorithm& endpointIndexerAlgorithm,
        _taskIndexerAlgorithm& taskIndexerAlgorithm) :
            taskToAgentAlgorithm(taskPathToAgentAlgorithm),
            chargingTaskToAgentAlgorithm(taskPathToAgentAlgorithm, endpointIndexerAlgorithm),
            selectRestEndpointToAgentAlgorithm(endpointIndexerAlgorithm, pathToAgentAlgorithm),
            selectChargingEndpointToAgentAlgorithm(endpointIndexerAlgorithm, pathToAgentAlgorithm),
            taskIndexerAlgorithm(taskIndexerAlgorithm) {}

_selectTaskSwapToAgentAlgorithm::_selectTaskSwapToAgentAlgorithm(const _selectTaskSwapToAgentAlgorithm& other) :
    selectChargingEndpointToAgentAlgorithm(other.selectChargingEndpointToAgentAlgorithm), 
    selectRestEndpointToAgentAlgorithm(other.selectRestEndpointToAgentAlgorithm), 
    taskToAgentAlgorithm(other.taskToAgentAlgorithm), 
    chargingTaskToAgentAlgorithm(other.chargingTaskToAgentAlgorithm), 
    taskIndexerAlgorithm(other.taskIndexerAlgorithm) { }

_selectTaskSwapToAgentAlgorithm::~_selectTaskSwapToAgentAlgorithm(){}

void _selectTaskSwapToAgentAlgorithm::setTaskIndexerAlgorithm(_taskIndexerAlgorithm& taskIndexerAlgorithm) {
    this->taskIndexerAlgorithm = taskIndexerAlgorithm;
}

void _selectTaskSwapToAgentAlgorithm::setEndpointIndexerAlgorithm(_endpointIndexerAlgorithm& endpointIndexerAlgorithm){
    chargingTaskToAgentAlgorithm.setEndpointIndexerAlgorithm(endpointIndexerAlgorithm);
    selectChargingEndpointToAgentAlgorithm.setEndpointIndexerAlgorithm(endpointIndexerAlgorithm);
    selectRestEndpointToAgentAlgorithm.setEndpointIndexerAlgorithm(endpointIndexerAlgorithm);
}

bool _selectTaskSwapToAgentAlgorithm::solve(
        const _token& token,
        const _agent& agent,
        _task& selectedTask,
        _stepPath& selectedPath, 
        std::vector<_swapInfo>& swapInfo) const {

    std::vector<_task> vtask;

    token.listNoRunningYetTasks([&vtask, &token, agent, this](const _task & task) {

        this->taskIndexerAlgorithm.solve(token, task, agent, vtask);

        return false;

    });

    for (auto task : vtask) {
        
        bool flag = false;
            
        _stepPath taskPath(selectedPath);

        _stepSite pickupSite, deliverySite;
        
        int otherAgentId = token.assignmentTaskAgentId(task.id());
        
        if(otherAgentId < 0){  
                        
            if(agent.isAtEnergyCriticalLevel()){
                
                flag = chargingTaskToAgentAlgorithm.solve(token, agent, task, taskPath, pickupSite, deliverySite);
                
            }else{
                
//                std::cout << token.getStepMap() << std::endl;
                                
                flag = taskToAgentAlgorithm.solve(token, agent, task, taskPath, pickupSite, deliverySite);
                
            }
            
            if (flag) {

                selectedTask = task;

                taskPath.pop();
                selectedPath.progress(taskPath);

                return true;

            }
                        
            
        } else {
                        
            _token* cloneToken = token.getClone();
            
            const _agent* swapAgent = cloneToken->agentById(agent.id());
            
            const _agent* otherAgent = cloneToken->agentById(otherAgentId);
            
            unsigned pickupDistOtherAgent = otherAgent->pickupStepToArrive();
            
            cloneToken->unassignTaskSwap(task.id(), otherAgentId);
            
            if(swapAgent->isAtEnergyCriticalLevel()){
                
                flag = chargingTaskToAgentAlgorithm.solve(*cloneToken, *swapAgent, task, taskPath, pickupSite, deliverySite);
                
            }else{
                
//                std::cout << cloneToken->getStepMap() << std::endl;
                                
                flag = taskToAgentAlgorithm.solve(*cloneToken, *swapAgent, task, taskPath, pickupSite, deliverySite);
                
            }
            
            if(flag){
                
                unsigned pickupDistSwapAgent = taskPath.stepToArrive(task.getPickup());
                
                flag = pickupDistSwapAgent < pickupDistOtherAgent;

                if (flag) {
                    
                    cloneToken->assignTaskSwap(task.id(), swapAgent->id(), taskPath);
                    
                    _task otherTask;
                    _stepPath otherPath(otherAgent->currentPath());

                    flag = solve(*cloneToken, *otherAgent, otherTask, otherPath, swapInfo);

                    if(flag){

                        selectedTask = task;
                        taskPath.pop();
                        selectedPath.progress(taskPath);

                        swapInfo.push_back(_swapInfo(task.id(), otherAgent->id(), otherPath, otherTask.id()));

                        delete cloneToken;
                        
                        return true;

                    } else {

                        _site otherEndpoint;
                        _stepPath endpointPath(selectedPath);
                        
                        if(otherAgent->isAtEnergyCriticalLevel()){
                            
                            flag = selectChargingEndpointToAgentAlgorithm.solve(*cloneToken, *otherAgent, otherEndpoint, endpointPath);
                            
                        } else {
                            
                            flag = selectRestEndpointToAgentAlgorithm.solve(*cloneToken, *otherAgent, otherEndpoint, endpointPath);
                            
                        }
                        
                        if(flag){

                            selectedTask = task;
                            taskPath.pop();
                            selectedPath.progress(taskPath);

                            swapInfo.push_back(_swapInfo(task.id(), otherAgent->id(), endpointPath));

                            delete cloneToken;
                            
                            return true;

                        }                          

                    }
                                        
                }
            
            } 
                        
            delete cloneToken;
            
        }

    }
    
    return false;

}