/* 
 * File:   AstarAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:49
 */

#ifndef ASTARALGORITHM_H
#define ASTARALGORITHM_H

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include "PathAlgorithm.h"
#include "SiteMap.h"

class AstarAlgorithm : public PathAlgorithm{
public:
        
    virtual bool solve(const SiteMap& map, const Site& start, const Site& goal, Path& path){
        
        this->closedStates.clear(); // row -> colunm        
        this->priorityStates.clear();
        
        AstarState* startState = new AstarState(.0f, this->heuristic(start, goal), start, nullptr);        
        AstarState* solved = this->solveAux(startState, map, goal);
                    
        while(solved != nullptr){            
            path.insert(0, solved->getSite());
            solved = solved->getPrevious();
        }
        
        
        return solved;
        
    }
    
    virtual ~AstarAlgorithm(){
        
        for (auto elem : visited) {
            
            delete elem;

        }
        
    }
    
protected:
    
    class AstarState{
    public:
        
        AstarState(float traveled, float heuristic, const Site& site, AstarState* previous) :
        traveled(traveled), _cost(heuristic + traveled), site(site), previous(previous) {}
        

        AstarState(const AstarState& other) :
        traveled(other.traveled), _cost(other._cost), site(other.site), previous(other.previous) { }
        
        virtual ~AstarState(){ }
        
        const Site getSite() const {
            return site;
        }
        
        float cost() const{
            return this->_cost; 
        }

        float getTraveled() const {
            return traveled;
        }
                
        bool operator<(const AstarState& right) const {
            return this->cost() < right.cost();
        }
                
        AstarState* getPrevious() const {
            return this->previous;
        }
        
    private:
        float traveled, _cost;
        Site site;
        AstarState* previous;
    };
    
    class AstarStateComparison {
    public:      
        bool operator() (const AstarState* as1, const AstarState* as2) const {
          return *as2 < *as1;
        }
    };
    
    
private:
    
    std::vector<AstarState*> visited;
    
    class PriorityStates{
        
    public:
        
        void push(AstarState*  state){
            
            this->states.push(state);
            
        }
        
        AstarState* pop(){
            auto ret = this->states.top();
            this->states.pop();
            return ret;
        }
        
        bool empty(){
            return this->states.empty();
        }
        
        void clear(){            
            while (!this->states.empty())
               this->states.pop();           
        }
        
        
    private:
        
        std::priority_queue<AstarState*,std::vector<AstarState*>,AstarStateComparison> states;
        
    } priorityStates;
    
    class ClosedStates{
    public:
        
        void add(AstarState* state){
            
            std::unordered_map<unsigned, std::unordered_set<unsigned>>::iterator it;
            it = this->states.find(state->getSite().row());

            if ( it == this->states.end() ){
                std::unordered_set<unsigned> set;
                it = this->states.insert(std::pair<unsigned, std::unordered_set<unsigned>>(state->getSite().row(), set)).first;
            } 
            
            it->second.insert(state->getSite().colunm());
            
        }
        
        bool closed(unsigned row, unsigned colunm){
            
            std::unordered_map<unsigned, std::unordered_set<unsigned>>::const_iterator it;
            it = this->states.find(row);

            if ( it != this->states.end() ){
                std::unordered_set<unsigned>::const_iterator it2;
                it2 = it->second.find(colunm);
                if(it2 != it->second.end()){
                    return true;
                }                
            } 
            
            return false;
            
        }
        
        void clear(){
            this->states.clear();
        }
        
    private:
        std::unordered_map<unsigned, std::unordered_set<unsigned>> states;
    } closedStates;
    
    virtual float heuristic(const Site& start, const Site& goal) const{
        
        return std::abs((int)start.row() - (int)goal.row()) + std::abs((int)start.colunm() - (int)goal.colunm());
        
    }    
    
    AstarState* solveAux(
        AstarState* current,
        const SiteMap& map,
        const Site& goal){
        
        this->visited.push_back(current);
                
        if(goal.match(current->getSite())){
                        
            return current;
            
        }
            
        this->closedStates.add(current);
        auto neighborhood = map.neighborhood(current->getSite());

        std::vector<Site>::const_iterator it;
        for(it = neighborhood.begin(); it != neighborhood.end(); it++){

            if(!closedStates.closed(it->row(), it->colunm())){

                this->priorityStates.push(new AstarState(current->getTraveled() + 1, this->heuristic(*it, goal), *it, current));

            }

        }
        
        if(!priorityStates.empty()){
            
            return this->solveAux(priorityStates.pop(), map, goal);
            
        }  
                
        
        return nullptr;
        
    }
    
};

#endif /* ASTARALGORITHM_H */

