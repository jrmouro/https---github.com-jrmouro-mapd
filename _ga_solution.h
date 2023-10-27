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
#include "_allocation.h"

class _endpointsDistanceAlgorithm;
class _agent_energy_system;

class _ga_solution : public _ga_pseudo_solution, public _allocation{
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
    
    virtual _allocation* clone() const;
    
    virtual bool isValid()const;
    
    virtual void nextPlanningUpdate(
        const _ga_token&,
        const std::function<bool(const _ga_agent* agent, const _task* task)>&);
    
    _ga_solution* randon(unsigned seed) const;
    
    virtual void disturb(unsigned seed);
    virtual void disturb(unsigned agents_size, unsigned task_size, unsigned seed);
    
    virtual _ga_solution* get_disturb(unsigned seed) const;    
    virtual _ga_solution* get_disturb(unsigned agents_size, unsigned task_size, unsigned seed) const;
    
    virtual std::pair<_ga_solution*,_ga_solution*> get_crossover(const _ga_solution& other) const;
    
    _ga_solution& operator=(const _ga_solution& right);
    
    friend std::ostream& operator<<(std::ostream& os, const _ga_solution& obj);
    
    bool isEvaluated()const;
    
    virtual bool dominate(const _ga_token& token, _ga_solution&);
    
    
    
private:
    
    std::map<const _ga_agent*,std::vector<const _task*>> allocation_map;
    std::map<EvalType, unsigned> evals;
    
    void alloc(const _ga_token& token);
    
};

#endif /* _GA_SOLUTION_H */

