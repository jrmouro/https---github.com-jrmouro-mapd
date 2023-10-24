/* 
 * File:   _ga_solution.h
 * Author: ronaldo
 *
 * Created on 17 de outubro de 2023, 09:55
 */

#ifndef _GA_SOLUTION_H
#define _GA_SOLUTION_H

#include <map>
#include <vector>
#include "_ga_pseudo_solution.h"

class _endpointsDistanceAlgorithm;
class _agent_energy_system;

class _ga_solution : public _ga_pseudo_solution{
public:
    
    enum EvalType{
        makespan,
        energy
    };
        
    _ga_solution();
    
    _ga_solution(const _ga_token& token);
    
    _ga_solution(const _ga_solution& other);

    virtual ~_ga_solution();
    
    const std::map<EvalType, unsigned>& evaluate(const _ga_token& token);
    
    unsigned evaluate(const _ga_token& token, const EvalType&);
    
    void planning(const _ga_token& token, const std::function<bool(const _ga_agent*, const _task*)>&);
    
    _ga_solution* randon(unsigned seed) const;
    
    virtual void disturb(unsigned seed);
    virtual void disturb(unsigned agents_size, unsigned task_size, unsigned seed);
    
    virtual _ga_solution* get_disturb(unsigned seed) const;    
    virtual _ga_solution* get_disturb(unsigned agents_size, unsigned task_size, unsigned seed) const;
    
    virtual std::pair<_ga_solution*,_ga_solution*> get_crossover(const _ga_solution& other) const{
        
        _ga_solution* childA = new _ga_solution();
        _ga_solution* childB = new _ga_solution();
        
        crossover(*this, other, *childA, *childB);
        
        return std::pair<_ga_solution*,_ga_solution*>(childA, childB);
        
    }
    
    virtual bool dominate(const _ga_token& token, const _ga_solution&) const;
    
private:
    
    std::map<const _ga_agent*,std::vector<const _task*>> allocation;
    std::map<EvalType, unsigned> evals;
    
    void alloc(const _ga_token& token);
    
};

#endif /* _GA_SOLUTION_H */

