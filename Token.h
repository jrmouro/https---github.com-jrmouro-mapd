/* 
 * File:   Token.h
 * Author: ronaldo
 *
 * Created on 2 de agosto de 2023, 20:01
 */

#ifndef TOKEN_H
#define TOKEN_H

class Token {
public:
    
    Token(const SiteMap& siteMap): siteMap(siteMap){}
    
    Token(const SiteMap& siteMap, const std::vector<Agent>& agents) : siteMap(siteMap),
    agents(agents) {}
    
    Token(const SiteMap& siteMap, const std::vector<Agent>& agents, const std::vector<Task>& tasks) :
    siteMap(siteMap), agents(agents), tasks(tasks) {}
    
    Token(const Token& other) :
    siteMap(other.siteMap), agents(other.agents), tasks(other.tasks) { }
    
    virtual ~Token(){}  
    
    void addAgent(const Agent& agent){
        this->agents.push_back(agent);
    }
    
    void addTask(const Task& task){
        this->tasks.push_back(task);
    }
    
    void listAgents(const std::function<bool(const Agent&)> function){
        
        for (auto agent : agents) {
            
            if(function(agent))return;

        }

    }
    
    void listTasks(const std::function<bool(const Task&)> function){
        
        for (auto task : tasks) {
            
            if(function(task))return;

        }

    }
    
    
    
private:
    std::vector<Agent> agents;
    std::vector<Task> tasks;
    SiteMap siteMap;
};

#endif /* TOKEN_H */

