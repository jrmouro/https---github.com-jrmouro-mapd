/* 
 * File:   _astarAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:49
 */

#ifndef _ASTARALGORITHM_H
#define _ASTARALGORITHM_H

#include <queue>
#include <iostream>
#include "_pathAlgorithm.h"
#include "_site.h"

class _site;
class _astarAlgorithm : public _pathAlgorithm{
public:
    
    _astarAlgorithm();
    
    _astarAlgorithm(const _astarAlgorithm& other);
    
    virtual bool solve(
            const _map& map, 
            const _site& start, 
            const _site& goal, 
            _path& path) const;
    
    virtual ~_astarAlgorithm();
    
protected:
        
    class AstarState{
    public:
        
        AstarState(float traveled, float heuristic, const _site& site, AstarState* previous);        

        AstarState(const AstarState& other);
        
        virtual ~AstarState();
        
        const _site getSite() const;
        
        float cost() const;

        float getTraveled() const;
                
        bool operator<(const AstarState& right) const;
                
        AstarState* getPrevious() const;
        
        friend std::ostream& operator<<(std::ostream& os, const AstarState& obj) {
            os << "site: " << obj.site;
            os << "  traveled: " << obj.traveled;
            os << "  cost: " << obj._cost << std::endl;
            return os;
        }

        
    private:
        float traveled, _cost;
        _site site;
        AstarState* previous;
    };
    
    class AstarStateComparison {
    public:      
        bool operator() (const AstarState* as1, const AstarState* as2) const;
    };
    
    
private:
        
    class PriorityStates{
        
    public:
        
        virtual ~PriorityStates();
        
        void push(AstarState*  state);
        
        AstarState* pop();
        
        bool empty();
        
        void clear();
        
        
    private:
        
        std::priority_queue<AstarState*,std::vector<AstarState*>,AstarStateComparison> states;
        
    };
    
    class ClosedStates{
    public:
        
        ClosedStates(unsigned colunm, unsigned size);
        
        ClosedStates(const ClosedStates& other);

        void add(AstarState* state);
        
        bool closed(const _site& site);
        
    private:
        unsigned colunm;
        std::vector<bool> states;
    };
    
    virtual float heuristic(const _site& start, const _site& goal) const;
    
    AstarState* solveAux_recursive(
        AstarState* current,
        const _map& map,
        const _site& goal,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        std::vector<AstarState*>& visitedStates) const;
    
    AstarState* solveAux_iterative(
        AstarState* current,
        const _map& map,
        const _site& goal,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        std::vector<AstarState*>& visitedStates) const;
    
};

#endif /* _ASTARALGORITHM_H */

