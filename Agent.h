/* 
 * File:   Agent.h
 * Author: ronaldo
 *
 * Created on 2 de agosto de 2023, 10:53
 */

#ifndef AGENT_H
#define AGENT_H

#include "Site.h"
#include "BinaryPath.h"
#include "BinaryMap.h"
#include "Token.h"
#include "Task.h"
#include "ManhattanAlgorithm.h"


class Agent : public Identifiable<int>{
    
public:
    
    enum State{ free, occupied };
            
    Agent(int _id, const Site& current) :  _id(_id), path(current) { }
    Agent(const Agent& other) : _id(other._id), state(other.state), path(other.path) { }
    
    virtual int id() const{
        return this->_id;
    }
    
    State getState() const {
        return state;
    }

    void setState(State state) {
        this->state = state;
    }
    
    virtual ~Agent(){
    
        if(this->task != nullptr) delete this->task;
    
    }
    
    void receive(Token& token){
        
        if(task == nullptr){ // agente livre
            
            this->updatePath(token);
            
        }        
                
    }
    
    void move(Token& token){
        
        BinarySite site = path.pop();
        
        if(task != nullptr && task->getDelivery().match(site)){
            
            token.addFinishedTask(*task);
            delete task;
            task = nullptr;
            
        }
        
        if(path.size() == 0){
            
            path.add(BinarySite(site.GetStep() + 1, site.GetRow(), site.GetColunm(), true));
            
        }   
        
        token.getBinaryMap().setMoving(site, path.currentSite());
        
    }
    
    const BinarySite& currentSite()const{
        
        return path.currentSite();
        
    }
    
    const BinarySite& goalSite()const{
        
        return path.goalSite();
        
    }
    
protected:
    
    virtual _site* selectNewEndpoint(Token& token){
        
        unsigned min_distance = 0xffffffff;
        _site* endpoint = nullptr;
        
        token.listEndpoints([token&, min_distance&, endpoint, this](const _site& site){
            
            token.listTasks([token&, min_distance&, endpoint, site&, this](const Task& task){
                
                if(!task.getDelivery().match(site)){                    
                    
                    token.listAgents([token&, min_distance&, endpoint, site&, this](const Agent& agent){
            
                        if(this->id() != agent.id()){ //other agents

                            if(!agent.goalSite().match(site)) {

                                unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), site);

                                if(distance < min_distance){
                                    min_distance = distance;
                                    if(endpoint != nullptr) delete endpoint;
                                    endpoint = new _site(task);
                                }

                            } 

                        } else {

                            unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), site);

                            if(distance < min_distance){
                                min_distance = distance;
                                if(endpoint != nullptr) delete endpoint;
                                endpoint = new _site(task);
                            }

                        }

                        return false;

                    });
                    
                    
                }
                
                return false;
                
            });
           
            
            return false;
            
        });
        
        return endpoint;
        
    }
      
    virtual Task* selectNewTask(Token& token) const {
                             
        unsigned min_distance = 0xffffffff;
        Task* selectedTask = nullptr;
        
        token.listTasks([token&, min_distance&, selectedTask, this](const Task& task){
            
            token.listAgents([token&, min_distance&, selectedTask, task&, this](const Agent& agent){
            
                if(this->id() != agent.id()){ //other agents
                    
                    if(!agent.goalSite().match(task.getPickup()) && !agent.goalSite().match(task.getDelivery())) {

                        unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), task.getPickup());

                        if(distance < min_distance){
                            min_distance = distance;
                            if(selectedTask != nullptr) delete selectedTask;
                            selectedTask = new Task(task);
                        }

                    } 
                    
                } else {
                    
                    unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), task.getPickup());

                    if(distance < min_distance){
                        min_distance = distance;
                        if(selectedTask != nullptr) delete selectedTask;
                        selectedTask = new Task(task);
                    }
                    
                }
            
                return false;
                
            });
            
            return false;
            
        });
        
        return selectedTask;
        
    }
    
    virtual void updateRestEndpointPath(Token& token){
        
        bool isEndpoint = false;
        
        token.listTasks([token&, isEndpoint&, this](const Task& task){
            
            if(task.getDelivery().match(this->currentSite())){
                
                isEndpoint = true;
                
                return true;
                
            }
            
            return false;
            
        });
        
        if(isEndpoint){
                     
            _site* endpoint = selectNewEndpoint(token);

            if(endpoint != nullptr){

                bool flag = token.pathAlgorithm()->solve(
                        token.getBinaryMap(), 
                        this->path.currentSite(), 
                        *endpoint, 
                        path, //pickupPath, 
                        token.getCurrentStep());

                delete endpoint;

                if(flag){

                    token.getBinaryMap().setMoving(path);          

                }else{

                    try {
                        std::ostringstream stream;
                        stream << "unsolved rest endpoint path";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }           

            } else {

                try {
                    std::ostringstream stream;
                    stream << "new rest endpoint not found";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }            

            }        
        
        }
        
    }
    
    virtual bool updateTaskPath(Token& token){
                     
        Task* task = selectNewTask(token);
        
        if(task != nullptr){
            
            if(this->task != nullptr) delete this->task;
            this->task = task;
            token.removePendingTask(*task);
                        
            bool flag = token.pathAlgorithm()->solve(
                    token.getBinaryMap(), 
                    this->path.currentSite(), 
                    task->getPickup(), 
                    path, //pickupPath, 
                    token.getCurrentStep());
                                    
            if(flag){
                
                BinaryPath deliveryPath;
                
                flag = token.pathAlgorithm()->solve(
                        token.getBinaryMap(), 
                        task->getPickup(), 
                        task->getDelivery(), 
                        deliveryPath, 
                        token.getCurrentStep() + path.size()() - 1);
                
                if(flag){
                    
                    deliveryPath.pop();
                    path.progress(deliveryPath);   
                    token.getBinaryMap().setMoving(path);
                    
                    return true;
                    
                } else {
                    
                    try {
                        std::ostringstream stream;
                        stream << "unsolved task delivery endpoint path";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }
                    
                }
                
            }else{
                
                try {
                    std::ostringstream stream;
                    stream << "unsolved task pickup endpoint path";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }
                
            }
            
        } 
        
        return false;
        
    }
       
    virtual void updatePath(Token& token){
                     
        bool flag = updateTaskPath(token);
        
        if(!flag){
            
            updateRestEndpointPath(token);
            
        } 
        
    }
    
private:
    int _id;
    State state = free;
    BinaryPath path;
    Task* task = nullptr;
};

#endif /* AGENT_H */

