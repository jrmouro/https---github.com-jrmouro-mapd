/* 
 * File:   _ga_agent.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 07:28
 */

#include "_ga_agent.h"
#include "_stepPath.h"
#include "_task.h"
#include "_ga_agent_state_free.h"

_ga_agent::_ga_agent(
        int id, 
        const _stepSite& initialSite, 
        const _agent_energy_system& agent_energy_system) : 
    _id(id),
    state(_ga_agent_state_free::getInstance()), 
    path(initialSite),
    agent_energy_system(agent_energy_system){}
    
_ga_agent::_ga_agent(const _ga_agent& other) :
    _id(other._id),
    state(other.state),     
    path(other.path),
    agent_energy_system(other.agent_energy_system){
    if(other.currentTask != nullptr)
        currentTask = new _task(*other.currentTask); 
}

_ga_agent::~_ga_agent(){
    if(currentTask != nullptr) delete currentTask;
}

int _ga_agent::id()const{
    return _id;
}

void _ga_agent::draw(const Render& render) const{
    
    this->state->onDraw(render, *this);
  
}

void _ga_agent::stepping(const _map& map){
        
    state->onStepping(map, *this);

} 

bool _ga_agent::isAtEnergyChargedLevel()const{
    return agent_energy_system.isAtChargedLevel();
}

bool _ga_agent::isAtEnergyCriticalLevel()const{
    return agent_energy_system.isAtCriticalLevel();
}  

bool _ga_agent::isAtEnergyDeadLevel()const{
    return agent_energy_system.isAtDeadLevel();
}

bool _ga_agent::isTaskAssigned()const{
    return this->currentTask != nullptr;
}

bool _ga_agent::isAtTrivialPath()const{
    return path.size() == 1;
}

bool _ga_agent::isDelivering()const{
    return isTaskAssigned() 
            && isAtTrivialPath() 
            && path.currentSite().match(currentTask->getDelivery());
}

bool _ga_agent::isPickuping() const {
    return isTaskAssigned() 
            && isAtTrivialPath() 
            && path.currentSite().match(currentTask->getPickup());
}

_stepSite _ga_agent::getGoalSite() const {
    return path.goalSite();
}

_stepSite _ga_agent::getCurrentSite() const {
    return path.currentSite();
}

_stepSite _ga_agent::getFutureSite() const {
    return path.futureSite();
}

const _agent_energy_system& _ga_agent::getAgentEnergySystem() const {
    return agent_energy_system;
}

const _task* _ga_agent::getCurrentTask() const {
    return currentTask;
}

void _ga_agent::assignTask(const _task& task) {
    if(this->currentTask != nullptr) delete this->currentTask;
    this->currentTask = new _task(task);
}

void _ga_agent::unassignTask() {
    if(this->currentTask != nullptr) delete this->currentTask;
    this->currentTask = nullptr;
}

void _ga_agent::assignPath(const _stepPath& path){
    this->path = path;
}

void _ga_agent::change_state(_ga_agent_state* state){
    this->state = state; 
}

bool _ga_agent::updateTaskPath(const _stepPathAlgorithm& astar, _stepMap& map, const _task* const task){
        
    if(isAtTrivialPath()){

        _site goal(path.currentSite().GetRow(), path.currentSite().GetColunm());

        if(isPickuping()){

            goal = currentTask->getDelivery();

        } else if(task != nullptr){

            goal = task->getPickup();
            assignTask(*task);

        } 

        return astar.solve(map, path, goal, id());

    }

    return false;

}
