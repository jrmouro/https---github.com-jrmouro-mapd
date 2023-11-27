/* 
 * File:   _stepAstarAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:49
 */

#ifndef _STEPASTARALGORITHM_H
#define _STEPASTARALGORITHM_H

#include <iostream>
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
    
    virtual bool solve(const _stepMap& map, _stepPath& path, const _site& goal, int type)  const {
        
        bool ret = false;
        
        if(!path.empty()){
             
            unsigned rowColunm = map.getRow_size()*map.getColumn_size();
            ClosedStates closedStates(rowColunm, map.getColumn_size() ,map.getStep_size() * rowColunm);
            PriorityStates priorityStates;
            std::vector<AstarState*> visitedStates;
            
            AstarState* startState = new AstarState(.0f, this->heuristic(path.goalSite(), goal), path.goalSite(), nullptr);
            
            AstarState* solved = this->solveAux_iterative(startState, map, goal, closedStates, priorityStates, visitedStates, type);
                  
            if(solved != nullptr){
                
                bool flag = map.isPathDefinitelyFree(solved->getSite(), type);
                
                if(flag){

                    _stepPath auxPath;

                    while(solved->getPrevious() != nullptr){            
                        auxPath.add(solved->getSite());
                        solved = solved->getPrevious();
                    } 

                    path.progress(auxPath);

                    ret = true;
                
                } 

            } else {
                
                map.max_step_view();
                map.free_agent_view();
                std::cout << std::endl;
                
                for (int i = 0; i < path.goalSite().GetStep() + 10; i++) {
                    map.stepView(i);                    
                    std::cout << std::endl;
                }
        
                
//                try {
//                    std::ostringstream stream;
//                    stream << "unsolved path from " << path.goalSite() << " to " << goal;
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                } 
                
            }
            
            for (auto elem : visitedStates) {
            
                delete elem;

            }
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "invalid empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            } 
             
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
            count++;
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
        
        unsigned getCount() const {
            return count;
        }
        
    private:
        unsigned count = 0;
        unsigned rowColunm, colunm;
        std::vector<bool> states;
    };
    
    virtual float heuristic(const _site& start, const _site& goal) const{
        
        return std::abs((int)start.GetRow() - (int)goal.GetRow()) + std::abs((int)start.GetColunm() - (int)goal.GetColunm());
        
    }    
        
        
    AstarState* solveAux_iterative(
        AstarState* current,
        const _stepMap& map,
        const _site& goal,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        std::vector<AstarState*>& visitedStates,
        int type) const {
        
        while(current != nullptr){
            
            visitedStates.push_back(current);
            
            if(goal.match(current->getSite()) && map.isPathDefinitelyFree(current->getSite(), type)){
                        
                return current;

            }
            
            map.listNeighborFreePaths(current->getSite(), type, [type, current, &goal, &closedStates, &priorityStates, this](const _stepSite& site){
            
            
                if(!closedStates.closed(site)){
                    
                    auto state = new AstarState(current->getTraveled() + 1, this->heuristic(site, goal), site, current);
                    
                    closedStates.add(state);
                    
                    priorityStates.push(state);
                    
                }

                return false;

            });
            
            if(!priorityStates.empty()){
            
                current = priorityStates.pop();
                                
            }  else {
                
                current = nullptr;
                
            }
                        
        }
                               
        return nullptr;               
        
    }
    
};

#endif /* _STEPASTARALGORITHM_H */

