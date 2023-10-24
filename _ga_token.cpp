/* 
 * File:   _ga_token.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 08:51
 */

#include "_ga_token.h"
#include "_ga_agent.h"
#include "_stepPath.h"
#include "_task.h"

_ga_token::_ga_token(
        const _map& map, 
        const _stepMap& stepMap,
        const _agent_energy_system& agent_energy_system):
    map(map),
    stepMap(stepMap),
    agent_energy_system(agent_energy_system){
    
    map.listBotsEndPoints([this, agent_energy_system](unsigned botId, const _site& endpoint){
            
        agents.insert(
            std::pair<unsigned, _ga_agent>(
                botId, 
                _ga_agent(
                    botId, 
                    _stepSite(0, endpoint.GetRow(), endpoint.GetColunm()), 
                    agent_energy_system)));

        return false;

    });
    
}

_ga_token::_ga_token(const _ga_token& other) :
    map(other.map),
    stepMap(other.stepMap),
    pendingTasks(other.pendingTasks), 
    assignedTasks(other.assignedTasks), 
    runningTasks(other.runningTasks), 
    finishedTasks(other.finishedTasks), 
    agents(other.agents), 
    assignTaskAgent(other.assignTaskAgent), 
    agent_energy_system(other.agent_energy_system), 
    currentStep(other.currentStep) { }


_ga_token::~_ga_token(){}  

void _ga_token::draw(const Render& render) const {

    for (const auto& pagent : agents) {

        pagent.second.draw(render);

    }

}

const _ga_agent& _ga_token::getAgentById(int agentId)const{
        
    std::map<int, _ga_agent>::const_iterator it = agents.find(agentId);

    if(it == agents.end()){

        try {
            std::ostringstream stream;
            stream << "agent id not found: " << agentId;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return it->second;

}

void _ga_token::writeHeader(std::ostream& fs) const {
    Writable::strWrite(*this, fs, "token_id", true); 
    Writable::strWrite(*this, fs, "token_name", true); 
    Writable::strWrite(*this, fs, "current_step", true);
    Writable::strWrite(*this, fs, "energy system id", true);
    Writable::strWrite(*this, fs, "energy expenditure", true);
    Writable::strWrite(*this, fs, "finished_tasks", false);
}   

void _ga_token::writeRow(std::ostream& fs) const {
    Writable::strWrite(*this, fs, id(), true); 
    Writable::strWrite(*this, fs, name(), true); 
    Writable::uintWrite(*this, fs, currentStep, true);
    Writable::strWrite(*this, fs, agent_energy_system.id(), true);
    Writable::intWrite(*this, fs, energyExpenditure(), true);
    Writable::uintWrite(*this, fs, finishedTasks.size(), false);
}

unsigned _ga_token::getCurrentStep() const {
    return currentStep;
}

void _ga_token::stepping(){
    
    for (auto pagent : agents) {
        
        pagent.second.stepping(map);       

    }
    
    currentStep++;
    
}

const _map& _ga_token::getMap() const {
    return map;
}

const _stepMap& _ga_token::getStepMap() const {
    return stepMap;
}

unsigned _ga_token::numberOfpendingTasks()const{
    return pendingTasks.size();
}

unsigned _ga_token::numberOfAgents()const{
    return agents.size();
}

std::string _ga_token::id() const{
    return "GAT";
}

std::string _ga_token::name() const {

    std::stringstream s;
    s << "GAT[" << agent_energy_system.id() << "]"; 

    return s.str();
}

int _ga_token::energyExpenditure() const {
    int ret = 0;
    listConstAgents([&ret](const _ga_agent& agent) {
        ret += (agent.getAgentEnergySystem().getCharging() - agent.getAgentEnergySystem().currentLevel());
        return false;
    });
    return ret;
}

bool _ga_token::isAgentDelivering(int agentId) const{
    return getAgentById(agentId).isDelivering();
}

bool _ga_token::isAgentPickuping(int agentId) const{
    return getAgentById(agentId).isPickuping();
}

bool _ga_token::isAgentAtTrivialPath(int agentId) const{
    return getAgentById(agentId).isAtTrivialPath();
}

bool _ga_token::isIdle()const{
    return  pendingTasks.empty() &&
            assignedTasks.empty() &&
            runningTasks.empty();
}

bool _ga_token::updateAgentTaskPath(
        int agentId, 
        int taskId, 
        const _stepPathAlgorithm& astar, 
        _stepMap& map){
    
    std::map<int, _ga_agent>::iterator agent_it = agents.find(agentId);
    
    if(agent_it != agents.end()){
        
        if(agent_it->second.isPickuping()){
                
            std::map<int, _task>::const_iterator currentTask = assignedTasks.find(agent_it->second.getCurrentTask()->id());

            if(currentTask != assignedTasks.end()){

                runningTasks.insert(std::pair<int, _task>(agent_it->second.getCurrentTask()->id(), currentTask->second));
                assignedTasks.erase(currentTask);

            } else {

                try {
                    std::ostringstream stream;
                    stream << "assigned task id not found: " << agent_it->second.getCurrentTask()->id();
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        } else if(agent_it->second.isDelivering()){

            std::map<int, _task>::const_iterator currentTask_it = runningTasks.find(agent_it->second.getCurrentTask()->id());

            if(currentTask_it != runningTasks.end()){

                finishedTasks.insert(std::pair<int, _task>(agent_it->second.getCurrentTask()->id(), currentTask_it->second));
                runningTasks.erase(currentTask_it);

            } else {

                try {
                    std::ostringstream stream;
                    stream << "running task id not found: " << agent_it->second.getCurrentTask()->id();
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        }
        
        std::map<int, _task>::const_iterator task_it = pendingTasks.find(taskId);
        
        if(task_it != pendingTasks.end()){
            
            assignTaskAgent.insert(std::pair<int, int>(taskId, agentId));
            assignedTasks.insert(std::pair<int, _task>(task_it->second.id(), task_it->second));
            pendingTasks.erase(task_it);
            
            return agent_it->second.updateTaskPath(astar, map, &task_it->second);
            
        } else {
            
            return agent_it->second.updateTaskPath(astar, map, nullptr);
            
        }
        
    } else {
        
        try {
            std::ostringstream stream;
            stream << "agent id not found: " << agentId;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    return false;
    
}

    
void _ga_token::addPendingTask(const _task& task){
    this->pendingTasks.insert(std::pair<unsigned, _task>(task.id(), task));
}

//bool _ga_token::assignTask(int taskId, int agentId, const _stepPath& taskPath){
//
//    std::map<int, _task>::const_iterator it = pendingTasks.find(taskId);
//
//    if(it != pendingTasks.end()){
//
//        std::map<int, _ga_agent>::iterator it2 = agents.find(agentId);
//
//        if(it2 != agents.end()){
//            
//            if(it2->second.isDelivering()){
//                
//                std::map<int, _task>::const_iterator it3 = runningTasks.find(it2->second.getCurrentTask()->id());
//                
//                if(it3 != runningTasks.end()){
//
//                    finishedTasks.insert(std::pair<int, _task>(it2->second.getCurrentTask()->id(), it3->second));
//                    runningTasks.erase(it3);
//
//                } else {
//
//                    try {
//                        std::ostringstream stream;
//                        stream << "assigned task id not found: " << it2->second.getCurrentTask()->id();
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }
//
//                }
//                
//            } else if(it2->second.isPickuping()){
//                
//                return false;
//                
//            }
//
//            assignTaskAgent.insert(std::pair<int, int>(taskId, agentId));
//            assignedTasks.insert(std::pair<int, _task>(taskId, it->second));
//            pendingTasks.erase(it);
//            
//            it2->second.assignPath(taskPath);
//            it2->second.assignTask(it->second);
//            
//            return true;
//                    
//
//        } else {                
//
//            try {
//                std::ostringstream stream;
//                stream << "agent id not found: " << agentId;
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//        }
//
//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "pending task id not found: " << taskId;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }
//    
//    return false;
//
//}

//void _ga_token::finishTask(int taskId, int agentId, const _stepPath& path){
//    
//    std::map<int, _task>::const_iterator it = assignedTasks.find(taskId);
//
//    if(it != assignedTasks.end()){
//
//        std::map<int, _ga_agent>::iterator it2 = agents.find(agentId);
//
//        if(it2 != agents.end()){
//
//            finishedTasks.insert(std::pair<int, _task>(taskId, it->second));
//            assignedTasks.erase(it);
//            
//            it2->second.assignPath(path);
//            it2->second.assignTask(nullptr);
//                    
//
//        } else {                
//
//            try {
//                std::ostringstream stream;
//                stream << "agent id not found: " << agentId;
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//        }
//
//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "assigned task id not found: " << taskId;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }
//    
//}

//bool _ga_token::runnigTask(int taskId, int agentId, const _stepPath& path){
//    
//    std::map<int, _task>::const_iterator it = assignedTasks.find(taskId);
//
//    if(it != assignedTasks.end()){
//
//        std::map<int, _ga_agent>::iterator it2 = agents.find(agentId);
//
//        if(it2 != agents.end()){
//            
//            if(it2->second.isPickuping()){
//                
//                runningTasks.insert(std::pair<int, _task>(taskId, it->second));
//                assignedTasks.erase(it);
//
//                it2->second.assignPath(path);
//                
//                return true;
//                
//            }                   
//
//        } else {                
//
//            try {
//                std::ostringstream stream;
//                stream << "agent id not found: " << agentId;
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//        }
//
//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "assigned task id not found: " << taskId;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }
//    
//    return false;
//    
//}


//void _ga_token::assignPath(int agentId, const _stepPath& path){
//    
//    std::map<int, _ga_agent>::iterator it2 = agents.find(agentId);
//
//    if(it2 != agents.end()){
//        
//        if(it2->second.isDelivering()){
//                
//            std::map<int, _task>::const_iterator it3 = runningTasks.find(it2->second.getCurrentTask()->id());
//
//            if(it3 != runningTasks.end()){
//
//                finishedTasks.insert(std::pair<int, _task>(it2->second.getCurrentTask()->id(), it3->second));
//                runningTasks.erase(it3);
//                it2->second.unassignTask();
//
//            } else {
//
//                try {
//                    std::ostringstream stream;
//                    stream << "delivery assigned task id not found: " << it2->second.getCurrentTask()->id();
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                }
//
//            }
//
//        } else if(it2->second.isPickuping()){
//            
//            std::map<int, _task>::const_iterator it4 = assignedTasks.find(it2->second.getCurrentTask()->id());
//
//            if(it4 != assignedTasks.end()){
//
//                runningTasks.insert(std::pair<int, _task>(it2->second.getCurrentTask()->id(), it4->second));
//                assignedTasks.erase(it4);
//
//            } else {
//
//                try {
//                    std::ostringstream stream;
//                    stream << "delivery assigned task id not found: " << it2->second.getCurrentTask()->id();
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                }
//
//            }
//            
//        } else {
//            
//            it2->second.assignTask(nullptr);
//            
//        }
//        
//        it2->second.assignPath(path);
//        
//        return true;        
//
//    } else {                
//
//        try {
//            std::ostringstream stream;
//            stream << "agent id not found: " << agentId;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }
//    
//    return false;
//    
//}

void _ga_token::listPendingTasks(const std::function<bool(const _task&)> function)const{
        
    for (auto taskPair : pendingTasks) {

        if(function(taskPair.second))return;

    }

}

void _ga_token::listAgents(const std::function<bool(_ga_agent&)> function){
    
    for (auto agentPair : agents) {

        if(function(agentPair.second))return;

    }
    
}

void _ga_token::listConstAgents(const std::function<bool(const _ga_agent&)> function)const{
    
    for (auto agentPair : agents) {

        if(function(agentPair.second))return;

    }
    
}