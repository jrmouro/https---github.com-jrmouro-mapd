/* 
 * File:   _selectBackwardTaskToAgentAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 12 de setembro de 2023, 10:00
 */

#include <vector>
#include "_selectBackwardTaskToAgentAlgorithm.h"
#include "_taskPathToAgentAlgorithm.h"
#include "_taskIndexerAlgorithm.h"
#include "_thresholdAlgorithm.h"
#include "_token.h"
#include "_agent.h"
#include "_task.h"
#include "_stepPath.h"
#include <limits>

_selectBackwardTaskToAgentAlgorithm::_selectBackwardTaskToAgentAlgorithm(
        const _taskPathToAgentAlgorithm& taskPathToAgentAlgorithm,
        _taskIndexerAlgorithm& taskIndexerAlgorithm,
        float pickup_threshold,float delivery_threshold) :
            taskPathToAgentAlgorithm(taskPathToAgentAlgorithm),
            taskIndexerAlgorithm(taskIndexerAlgorithm),
            pickup_threshold(pickup_threshold),
            delivery_threshold(delivery_threshold){}

_selectBackwardTaskToAgentAlgorithm::_selectBackwardTaskToAgentAlgorithm(
        const _selectBackwardTaskToAgentAlgorithm& other) :
            taskPathToAgentAlgorithm(other.taskPathToAgentAlgorithm),
            taskIndexerAlgorithm(other.taskIndexerAlgorithm),
            pickup_threshold(other.pickup_threshold),
            delivery_threshold(other.delivery_threshold){}

void _selectBackwardTaskToAgentAlgorithm::setTaskIndexerAlgorithm(
        _taskIndexerAlgorithm& taskIndexerAlgorithm) {

    this->taskIndexerAlgorithm = taskIndexerAlgorithm;

}

bool _selectBackwardTaskToAgentAlgorithm::solve(
        const _token& token, 
        const _agent& agent, 
        _task& originalTask, 
        _task& selectedTask, 
        _task& pendingTask,
        _stepPath& selectedPath) const {

    _thresholdAlgorithm thresholdAlgorithm(token.getEndpointsPathAlgorithm());

    bool ret = false;

    std::vector<_task> vtask;

    token.listPendingTasks([&vtask, &token, agent, this](const _task & task) {

        this->taskIndexerAlgorithm.solve(token, task, agent, vtask);

        return false;

    });
    
    bool aux = false;
    _task auxTask;
    _stepPath auxPath;    
    float best_dist = std::numeric_limits<float>::max();

    for (auto task : vtask) {

        ret = true;

        token.listConstAgents([task, &ret, agent](const _agent& otherAgent) {

            if (otherAgent.id() != agent.id()) {

                if (otherAgent.goalSite().match(task.getPickup()) || otherAgent.goalSite().match(task.getDelivery())) {

                    ret = false;

                    return true;

                }

            }

            return false;

        });
        
        if (ret) {

            originalTask = task;

            _stepPath taskPath(selectedPath);

            _stepSite pickupSite, deliverySite;
            
            unsigned pickup_step = 0;
            float pickupRate = .0;

            ret = taskPathToAgentAlgorithm.solve(token, agent, originalTask, taskPath, pickupSite, deliverySite);
            
            pickup_step = pickupSite.GetStep() - taskPath.currentSite().GetStep();
                
            thresholdAlgorithm.solve(taskPath.currentSite(), originalTask.getPickup(), pickup_step, pickup_threshold, pickupRate);

            if (ret) {
                
                taskPath.backward(
                        [&ret, &token, agent, &taskPath, thresholdAlgorithm, 
                        pickupSite, deliverySite, &originalTask, &selectedTask, 
                        &pendingTask, &selectedPath, this](const _stepSite & site) {

                    if (site.step_match(pickupSite)) return true;

                    if (token.isTaskEndpoint(site) && token.getStepMap().isPathDefinitelyFree(site, agent.id())) {

                        ret = true;                       

                        token.listConstAgents([&ret, site, agent](const _agent& otherAgent) { // verifica se o task endpoint está disponível

                            if (otherAgent.id() != agent.id()) {

                                if (otherAgent.goalSite().match(site)) {

                                    ret = false;

                                    return true;

                                }

                            }

                            return false;

                        });

                        if (ret) {

                            unsigned step = site.GetStep() - pickupSite.GetStep();
                            
                            float deliveryRate;

                            ret = thresholdAlgorithm.solve(pickupSite, site, step, this->delivery_threshold, deliveryRate);

                            if (ret) {

                                selectedTask.setPickup(pickupSite);
                                selectedTask.setDelivery(site);
                                
                                _stepPath backwardTaskPath;
                                
                                taskPath.forward([site, &backwardTaskPath](const _stepSite& tsite) {

                                    backwardTaskPath.progress(tsite);

                                    if (tsite.step_match(site)) {

                                        return true;

                                    }

                                    return false;

                                });
                                
                                ret = agent.isAbleToFulfillTaskPath(token.getMap(), selectedTask, backwardTaskPath);

                                if (ret) {
                                    
                                    pendingTask.setPickup(site);
                                    pendingTask.setDelivery(deliverySite);
                                    
                                    backwardTaskPath.pop();
                                    selectedPath.progress(backwardTaskPath);
                                    
                                    return true;

                                }                                

                            }

                        }
                       
                    }

                    return false;

                }); 
                
            }
            
            if(!ret){
            
                float best_dist_aux = pickupRate * pickup_step;

                if(!aux){

                    aux = true;
                    auxTask = task;
                    auxPath = taskPath; 
                    best_dist = best_dist_aux;

                } else {

                    if(best_dist_aux < best_dist){

                        auxTask = task;
                        auxPath = taskPath;
                        best_dist = best_dist_aux;

                    } 

                }
            
            }

        }
        
        if(ret) return true;

    }
    
    if(aux){
        
        aux = agent.isAbleToFulfillTaskPath(token.getMap(), auxTask, auxPath);

        if (aux) {
            
            originalTask = auxTask;
            selectedTask = auxTask;

            auxPath.pop();
            selectedPath.progress(auxPath);

            return true;

        }
        
    }

    return false;

}