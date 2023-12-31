/* 
 * File:   _ga_solution.h
 * Author: ronaldo
 *
 * Created on 17 de outubro de 2023, 09:55
 */

#ifndef _GA_SOLUTION_H
#define _GA_SOLUTION_H

#include <climits>
#include <map>
#include <vector>
#include <random>
#include "_ga_pseudo_solution.h"
#include "_closerTaskIndexerAlgorithm.h"
#include "_allocation.h"

class _agent_energy_system;

class _ga_solution : public _ga_pseudo_solution, public _allocation{
public:
    
    enum EvalType{
        makespan,
        energy,
        balanced
    };
        
    _ga_solution(unsigned validity);
    
    _ga_solution(const _ga_token&, unsigned validity);
    
    _ga_solution(const _ga_solution&);

    virtual ~_ga_solution();
        
    virtual _allocation* clone() const;
    
    virtual bool isValid()const;
    
    virtual unsigned validity() const {
        return std::max<unsigned>(0, (int)_validity - (int)age);
    }
    
    virtual void revalidate();
    
    virtual void nextPlanningUpdate(
        const _ga_token&,
        const std::function<bool(const _ga_agent*, const _task*)>&);
    
    _ga_solution* randon(std::default_random_engine&) const;
    
    virtual void disturb(std::default_random_engine&);
    virtual void disturb(unsigned agents_size, unsigned task_size, std::default_random_engine&);
    
    virtual _ga_solution* get_disturb(std::default_random_engine&) const;    
    virtual _ga_solution* get_disturb(unsigned agents_size, unsigned task_size, std::default_random_engine&) const;
    
    virtual std::pair<std::pair<_ga_solution*,_ga_solution*>,std::pair<_ga_solution*,_ga_solution*>> get_crossover(
        const _ga_solution& other,
        unsigned agentsCrossoverPoint,
        unsigned tasksCrossoverPoint) const;
    
    _ga_solution& operator=(const _ga_solution& right);
    
    friend std::ostream& operator<<(std::ostream& os, const _ga_solution& obj);
    
    bool isEvaluated()const;
    bool isAllocated()const;
    
    virtual bool isDominatedBy(const _ga_token&, _ga_solution&);
    
//    unsigned getValidity() const {
//        return validity;
//    }

//    void setValidity(unsigned validity) {
//        this->validity = validity;
//    }
    
    virtual bool check(const _ga_token&) const;
    virtual void restore(const _ga_token&);
    
    const std::map<EvalType, unsigned>& getEvals() const {
        return evals;
    }

    void greedySolution(const _ga_token& token);
    void greedySolution_e(const _ga_token& token);
    
private:
    friend class _ga_real_of;
//    friend class _ga_estimate_of;
//    friend class _ga_estimate_of_path_count;
//    friend class _ga_estimate_of_path_check;
//    friend class _ga_estimate_of_path_collision;
//    friend class _ga_estimate_of_path_task_endpoint;
    std::map<EvalType, unsigned> evals;
    
private:
//    friend class _ga_estimate_of;    
    friend class _ga_estimative_of_path;
    friend class _ga_estimate_of_path_count;
    friend class _ga_estimate_of_path_check;
    friend class _ga_estimate_of_path_collision;
    friend class _ga_estimate_of_path_task_endpoint;
    std::map<const _ga_agent*,std::vector<const _task*>> allocation_map;
    
private:
    
    unsigned _validity, age = 0;
    
    
    virtual void alloc(const _ga_token& token);
    virtual void alloc2(const _ga_token& token);    
    virtual const std::map<EvalType, unsigned>& evaluateAux(const _ga_token& token);
    virtual const std::map<EvalType, unsigned>& evaluateAux2(const _ga_token& token);
    
};

#endif /* _GA_SOLUTION_H */

