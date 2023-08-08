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
#include "BinaryMap.h"
#include "Site.h"
#include "_site.h"

class AstarAlgorithm : public PathAlgorithm{
public:
    
    AstarAlgorithm(){}
    
    AstarAlgorithm(const AstarAlgorithm& other){}
    
    virtual bool solve(const BinaryMap& map, const _site& start, const _site& goal, BinaryPath& path, unsigned step) const{
        
        bool ret = false;      
        ClosedStates closedStates;
        PriorityStates priorityStates;
        std::vector<AstarState*> visitedStates;
        
        auto bstart = BinarySite(step, start.GetRow(), start.GetColunm(), true);
        
        AstarState* startState = new AstarState(.0f, this->heuristic(start, goal), bstart, nullptr);        
        AstarState* solved = this->solveAux(startState, map, goal, closedStates, priorityStates, visitedStates);
                    
        if(solved != nullptr){
            
            path.clear();
        
            while(solved != nullptr){            
                path.add(solved->getSite());
                solved = solved->getPrevious();
            }            
            
            ret = true;
        
        }
        
        for (auto elem : visitedStates) {
            
            delete elem;

        }
                
        return ret;
        
    }
    
    virtual ~AstarAlgorithm(){ }
    
protected:
        
    class AstarState{
    public:
        
        AstarState(float traveled, float heuristic, const BinarySite& site, AstarState* previous) :
        traveled(traveled), _cost(heuristic + traveled), site(site), previous(previous) {}
        

        AstarState(const AstarState& other) :
        traveled(other.traveled), _cost(other._cost), site(other.site), previous(other.previous) { }
        
        virtual ~AstarState(){ }
        
        const BinarySite getSite() const {
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
        BinarySite site;
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
        
    };
    
    class ClosedStates{
    public:
        
        void add(AstarState* state){
            
            std::unordered_map<unsigned, std::unordered_set<unsigned>>::iterator it;
            it = this->states.find(state->getSite().GetRow());

            if ( it == this->states.end() ){
                std::unordered_set<unsigned> set;
                it = this->states.insert(std::pair<unsigned, std::unordered_set<unsigned>>(state->getSite().GetRow(), set)).first;
            } 
            
            it->second.insert(state->getSite().GetColunm());
            
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
    };
    
    virtual float heuristic(const _site& start, const _site& goal) const{
        
        return std::abs((int)start.GetRow() - (int)goal.GetRow()) + std::abs((int)start.GetColunm() - (int)goal.GetColunm());
        
    }    
    
    AstarState* solveAux(
        AstarState* current,
        const BinaryMap& map,
        const _site& goal,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        std::vector<AstarState*>& visitedStates )const{
        
        visitedStates.push_back(current);
                
        if(goal.GetRow() == current->getSite().GetRow() && goal.GetColunm() == current->getSite().GetColunm()){
                        
            return current;
            
        }
            
        closedStates.add(current);
        auto neighborhood = map.neighborhood(current->getSite());

        std::vector<BinarySite>::const_iterator it;
        for(it = neighborhood.begin(); it != neighborhood.end(); it++){

            if(!closedStates.closed(it->GetRow(), it->GetColunm())){

                priorityStates.push(new AstarState(current->getTraveled() + 1, this->heuristic(*it, goal), *it, current));

            }

        }
        
        if(!priorityStates.empty()){
            
            return this->solveAux(priorityStates.pop(), map, goal,closedStates, priorityStates, visitedStates);
            
        }  
                   
        return nullptr;
        
    }
    
};

#endif /* ASTARALGORITHM_H */

