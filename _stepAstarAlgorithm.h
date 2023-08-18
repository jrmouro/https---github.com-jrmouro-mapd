/* 
 * File:   _stepAstarAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:49
 */

#ifndef _STEPASTARALGORITHM_H
#define _STEPASTARALGORITHM_H

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <iostream>
#include "_stepPathAlgorithm.h"
#include "_stepMap.h"
#include "_site.h"

class _stepAstarAlgorithm : public _stepPathAlgorithm{
public:
    
    _stepAstarAlgorithm(){}
    
    _stepAstarAlgorithm(const _stepAstarAlgorithm& other){}
    
    virtual bool solve(const _stepMap& map, const _site& start, const _site& goal, _stepPath& path, unsigned step, int type) const{
        
        bool ret = false;  
        unsigned rowColunm = map.getRow_size()*map.getColumn_size();
        ClosedStates closedStates(rowColunm, map.getColumn_size() ,map.getStep_size() * rowColunm);
        PriorityStates priorityStates;
        std::vector<AstarState*> visitedStates;
        
        auto bstart = _stepSite(step, start.GetRow(), start.GetColunm());
        
        AstarState* startState = new AstarState(.0f, this->heuristic(start, goal), bstart, nullptr);        
        AstarState* solved = this->solveAux(startState, map, goal, closedStates, priorityStates, visitedStates, type);
                  
//        std::cout << map << std::endl;
        
        if(solved != nullptr){
            
            path.clear();
        
            while(solved != nullptr){            
                path.add(solved->getSite());
                solved = solved->getPrevious();
            } 
            
//            path.pop(); // retira o site inicial
            
            ret = true;
        
        }
        
        for (auto elem : visitedStates) {
            
            delete elem;

        }
        
        
                
        return ret;
        
    }
    
    virtual ~_stepAstarAlgorithm(){ }
    
protected:
        
    class AstarState{
    public:
        
        AstarState(float traveled, float heuristic, const _stepSite& site, AstarState* previous) :
        traveled(traveled), _cost(heuristic + traveled), site(site), previous(previous) {}
        

        AstarState(const AstarState& other) :
        traveled(other.traveled), _cost(other._cost), site(other.site), previous(other.previous) { }
        
        virtual ~AstarState(){ }
        
        const _stepSite getSite() const {
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
        
        friend std::ostream& operator<<(std::ostream& os, const AstarState& obj) {
            os << "site: " << obj.site;
            os << "  traveled: " << obj.traveled;
            os << "  cost: " << obj._cost << std::endl;
            return os;
        }

        
    private:
        float traveled, _cost;
        _stepSite site;
        AstarState* previous;
    };
    
    class AstarStateComparison {
    public:      
        bool operator() (const AstarState* as1, const AstarState* as2) const {
          return *as2 < *as1;
        }
    };
    
    
private:
        
    class PriorityStates{
        
    public:
        
        virtual ~PriorityStates(){
            
            while (!this->states.empty()){
                delete states.top();
               this->states.pop();
            }
            
        }
        
        void push(AstarState*  state){
            
            this->states.push(state);
            
        }
        
        AstarState* pop(){
            if(this->states.empty())
                return nullptr;
            
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
        
    };
    
    class ClosedStates{
    public:
        
        ClosedStates(unsigned rowColunm, unsigned colunm, unsigned size) :
        rowColunm(rowColunm), colunm(colunm), states(size, false) {}
        
        ClosedStates(const ClosedStates& other) :
        rowColunm(other.rowColunm), colunm(other.colunm), states(other.states) { }

        void add(AstarState* state){
            states[
                state->getSite().GetStep() * rowColunm + 
                state->getSite().GetRow() * colunm + 
                state->getSite().GetColunm()] = true;             
        }
        
        bool closed(const _stepSite& site){
                        
            return states[
                site.GetStep() * rowColunm + 
                site.GetRow() * colunm + 
                site.GetColunm()];
            
        }
        
    private:
        unsigned rowColunm, colunm;
        std::vector<bool> states;
    };
    
    virtual float heuristic(const _site& start, const _site& goal) const{
        
        return std::abs((int)start.GetRow() - (int)goal.GetRow()) + std::abs((int)start.GetColunm() - (int)goal.GetColunm());
        
    }    
    
    AstarState* solveAux(
        AstarState* current,
        const _stepMap& map,
        const _site& goal,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        std::vector<AstarState*>& visitedStates,
        int type)const{
        
        visitedStates.push_back(current);
                
        if(goal.GetRow() == current->getSite().GetRow() && goal.GetColunm() == current->getSite().GetColunm()){
                        
            return current;
            
        }
            
        map.listNeighborFreePaths(current->getSite(), type, [current, goal, &closedStates, &priorityStates, this](const _stepSite& site){
            
            if(!closedStates.closed(site)){
                
                auto state = new AstarState(current->getTraveled() + 1, this->heuristic(site, goal), site, current);

                priorityStates.push(state);
                
                closedStates.add(state);

            }
         
            return false;
        });
        
        if(!priorityStates.empty()){
            
            auto state = priorityStates.pop();
            
            return this->solveAux(state, map, goal, closedStates, priorityStates, visitedStates, type);
            
        }  
                   
        return nullptr;
        
    }
    
};

#endif /* _STEPASTARALGORITHM_H */
