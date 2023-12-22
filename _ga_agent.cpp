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
#include "_ga_agent_state_buzy.h"

_ga_agent::_ga_agent(
        int id,
        const _stepSite& initialSite,
        const _agent_energy_system& agent_energy_system) :
            _id(id),
            _state(_ga_agent_state_free::getInstance()),
            _path(initialSite),
            _previousSite(initialSite),
            agent_energy_system(agent_energy_system) { }

_ga_agent::_ga_agent(const _ga_agent& other) :
        _id(other._id),
        _state(other._state),
        _path(other._path),
        _previousSite(other._previousSite),
        agent_energy_system(other.agent_energy_system) {
    if (other._currentTask != nullptr)
        _currentTask = new _task(*other._currentTask);
}

_ga_agent::~_ga_agent() {
    if (_currentTask != nullptr) delete _currentTask;
    if(r_path != nullptr){
        
        delete r_path;
        delete r_previousSite;

    }
}

int _ga_agent::id()const {
    return _id;
}

void _ga_agent::draw(const Render& render) const {

    this->_state->onDraw(render, *this);

}

void _ga_agent::stepping(const _map& map) {

    _state->onStepping(map, *this);
    
    if (_path.size() > 1) {

        this->_previousSite = _path.pop();   

    } else {

        try {
            std::ostringstream stream;
            stream << "invalid path size - agent: " << std::endl << *this;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

}

bool _ga_agent::isFree()const {
    return this->_state->id() == 0;
}

bool _ga_agent::isBuzy()const {
    return this->_state->id() == 1;
}

void _ga_agent::setStateFree(){
    _state = _ga_agent_state_free::getInstance();
}

void _ga_agent::setStateBuzy(){
    _state = _ga_agent_state_buzy::getInstance();
}

bool _ga_agent::isAtEnergyChargedLevel()const {
    return agent_energy_system.isAtChargedLevel();
}

bool _ga_agent::isAtEnergyCriticalLevel()const {
    return agent_energy_system.isAtCriticalLevel();
}

bool _ga_agent::isAtEnergyDeadLevel()const {
    return agent_energy_system.isAtDeadLevel();
}

bool _ga_agent::isTaskAssigned()const {
    return this->_currentTask != nullptr;
}

bool _ga_agent::isAtTrivialPath()const {
    return _path.size() == 1;
}

bool _ga_agent::isAtDelivering()const {

    return 
//            isBuzy()
//            && isAtTrivialPath()
            isTaskAssigned()
            && _path.currentSite().match(_currentTask->getDelivery());

}

bool _ga_agent::isAtPickuping()const {

    return 
//            isFree()
//            && isAtTrivialPath()
            isTaskAssigned()
            && _path.currentSite().match(_currentTask->getPickup());
}

bool _ga_agent::isAtResting()const{
    
    return 
//            isFree()            
//            && isAtTrivialPath() ;
            !isTaskAssigned();

}

bool _ga_agent::isAtRestPickuping()const{
    
    return 
//            isFree()
//            && isAtTrivialPath()
            isTaskAssigned()
            && !_path.currentSite().match(_currentTask->getPickup());
}


bool _ga_agent::isAtRestDelivering()const{
    
    return 
//            isBuzy() 
//            && isAtTrivialPath() ;
            isTaskAssigned()      
            && !_path.currentSite().match(_currentTask->getDelivery());
    
}

bool _ga_agent::isGoingToDelivering()const {
    
    return 
//            isBuzy()             
//            && !isAtTrivialPath() ;
            isTaskAssigned()
            && _path.goalSite().match(_currentTask->getDelivery());
}

bool _ga_agent::isGoingToPickuping() const {
    
    return 
//            isFree()            
//            && !isAtTrivialPath() ;
            isTaskAssigned()
            && _path.goalSite().match(_currentTask->getPickup());
}

bool _ga_agent::isGoingToResting() const {
    
    return 
//            isFree()
//            && !isAtTrivialPath() ;
            !isTaskAssigned();
}

bool _ga_agent::isGoingToRestDelivering() const {
    
    return 
//            isBuzy()            
//            && !isAtTrivialPath() ;
            isTaskAssigned()
            && !_path.goalSite().match(_currentTask->getDelivery());
}

bool _ga_agent::isGoingToRestPickuping() const {
    
    return 
//            isFree()            
//            && isAtTrivialPath() ;
            isTaskAssigned()
            && !_path.goalSite().match(_currentTask->getPickup());
}

const _stepSite& _ga_agent::goalSite() const {
    return _path.goalSite();
}

const _stepSite& _ga_agent::currentSite() const {
    return _path.currentSite();
}

const _stepSite& _ga_agent::getFutureSite() const {
    return _path.futureSite();
}

const _agent_energy_system& _ga_agent::getAgentEnergySystem() const {
    return agent_energy_system;
}

const _task* _ga_agent::getCurrentTask() const {
    return _currentTask;
}

void _ga_agent::assignTask(const _task& task) {
    if (this->_currentTask != nullptr) delete this->_currentTask;
    this->_currentTask = new _task(task);
}

void _ga_agent::unassignTask() {
    if (this->_currentTask != nullptr) delete this->_currentTask;
    this->_currentTask = nullptr;
}

void _ga_agent::assignPath(const _stepPath& path) {
    this->_path = path;
}
void _ga_agent::progressPath(const _stepPath& path) {
    this->_path.progress(path);
}

const _stepPath& _ga_agent::getPath() const {
    return _path;
}

void _ga_agent::change_state(_ga_agent_state* state) {
    this->_state = state;
}

bool _ga_agent::updateTaskPath(const _stepPathAlgorithm& astar, _stepMap& map, const _task * const task) {

    if (isAtTrivialPath()) {

        _site goal(_path.currentSite().GetRow(), _path.currentSite().GetColunm());

        if (isGoingToPickuping()) {

            goal = _currentTask->getDelivery();

        } else if (task != nullptr) {

            goal = task->getPickup();
            assignTask(*task);

        }

        return astar.solve(map, _path, goal, id());

    }

    return false;

}

std::ostream& operator<<(std::ostream& os, const _ga_agent& obj) {
    os << "id: " << obj.id();
    return os;
}
