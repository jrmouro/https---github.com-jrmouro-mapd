/* 
 * File:   _ga_token.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 08:51
 */

#ifndef _GA_TOKEN_H
#define _GA_TOKEN_H

#include <map>
#include <functional>
#include "_stepPathAlgorithm.h"
#include "Identifiable.h"
#include "Drawable.h"
#include "Writable.h"
#include "_ga_agent.h"

class _task;
class _stepPath;
class _ga_agent;
class __stepPathAlgorithm;

class _ga_token  : public Identifiable<std::string>, public Drawable, public Writable{
public:
    
    _ga_token(
            const _map& map, 
            const _stepMap& stepMap,
            const _agent_energy_system& agent_energy_system);  
    
    _ga_token(const _ga_token& other);   
    virtual ~_ga_token();
    
    virtual std::string id() const;    
    virtual std::string name() const;    
    int energyExpenditure() const;
    
    virtual void draw(const Render&) const;
    
    void addPendingTask(const _task&);
    
    bool updateAgentTaskPath(int, int, const _stepPathAlgorithm&, _stepMap&);
    
    
    void listPendingTasks(const std::function<bool(const _task&)> function) const;    
    void listAgents(const std::function<bool(_ga_agent&)> function);
    void listConstAgents(const std::function<bool(const _ga_agent&)> function) const;
    
    virtual bool isIdle() const;
    
    const _ga_agent& getAgentById(int agentId) const;
    
    bool isAgentDelivering(int agentId)const;
    bool isAgentPickuping(int agentId)const;
    bool isAgentAtTrivialPath(int agentId)const;
    
    
    virtual void writeHeader(std::ostream& fs) const;    
    virtual void writeRow(std::ostream& fs) const;   
    
    unsigned getCurrentStep() const;    
    void stepping();    
    unsigned numberOfpendingTasks()const;    
    unsigned numberOfAgents()const;    
    const _map& getMap() const;
    const _stepMap& getStepMap() const;

    const _agent_energy_system& getAgent_energy_system() const {
        return agent_energy_system;
    }

    
private:
    const _map& map;
    _stepMap stepMap;
    std::map<int, _task> pendingTasks, assignedTasks, runningTasks, finishedTasks;
    std::map<int, _ga_agent> agents;
    std::map<int, int> assignTaskAgent;
    const _agent_energy_system& agent_energy_system;
    unsigned currentStep = 0;
};

#endif /* _GA_TOKEN_H */

