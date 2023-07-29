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
    
    class AstarState{
    public:
        
        AstarState(float traveled, float heuristic, const Site& site, unsigned step) :
        traveled(traveled), _cost(heuristic + traveled), site(site), step(step) {}

        AstarState(const AstarState& other) :
        traveled(other.traveled), _cost(other._cost), site(other.site), step(other.step) { }
        
        virtual ~AstarState(){}
        
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
        
        unsigned getStep() const {
            return step;
        }
        
    private:
        unsigned step;
        float traveled, _cost;
        Site site;
    };
    
    class AstarStateComparison {
    public:      
        bool operator() (const AstarState& as1, const AstarState& as2) const {
          return as1 < as2;
        }
    };
    
    class PriorityStates{
        
    public:
        
        void push(const AstarState&  state){
            
            this->states.push(state);
            
        }
        
        AstarState pop(){
            auto ret = this->states.top();
            this->states.pop();
            return ret;
        }
        
        bool empty(){
            return this->states.empty();
        }
        
        
    private:
        
        std::priority_queue<AstarState,std::vector<AstarState>,AstarStateComparison> states;
        
    };
    
    class ClosedStates{
    public:
        
        void add(const AstarState& state){
            
            std::unordered_map<unsigned, std::unordered_set<unsigned>>::iterator it;
            it = this->states.find(state.getSite().row());

            if ( it == this->states.end() ){
                std::unordered_set<unsigned> set;
                it = this->states.insert(std::pair<unsigned, std::unordered_set<unsigned>>(state.getSite().row(), set)).first;
            } 
            
            it->second.insert(state.getSite().colunm());
            
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
        
    private:
        std::unordered_map<unsigned, std::unordered_set<unsigned>> states;
    };
    
    virtual float heuristic(const Site& start, const Site& goal) const{
        
        return std::abs((int)start.row() - (int)goal.row()) + std::abs((int)start.colunm() - (int)goal.colunm());
        
    }
    
    virtual bool solve(const SiteMap& map, const Site& start, const Site& goal, Path& path) const {
        
        ClosedStates closedStates; // row -> colunm        
        PriorityStates priorityStates;
        AstarState startState(.0f, this->heuristic(start, goal), start, 0);
        
        bool solved = this->solveAux(startState, closedStates, priorityStates, map, goal, path);
        
        if(!solved) path.clear();
        
        return solved;
        
    }
    
    virtual bool solveAux(
        const AstarState& current,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        const SiteMap& map,
        const Site& goal, 
        Path& path) const {
        
        path.set(current.getStep(), current.getSite());
        
        if(goal.match(current.getSite())){
                        
            return true;
            
        }
            
        closedStates.add(current);
        auto neighborhood = map.neighborhood(current.getSite());

        std::vector<Site>::const_iterator it;
        for(it = neighborhood.begin(); it != neighborhood.end(); it++){

            if(!closedStates.closed(it->row(), it->colunm())){

                priorityStates.push(AstarState(current.getTraveled() + 1, this->heuristic(*it, goal), *it, current.getStep() + 1));

            }

        }
        
        if(!priorityStates.empty()){
            
            return this->solveAux(priorityStates.pop(), closedStates, priorityStates, map, goal, path);
            
        }  
                
        
        return false;
        
    }
    
};

#endif /* ASTARALGORITHM_H */

