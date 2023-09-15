/* 
 * File:   _closerCooperatorAgentIndexerAlgorithm.h
 * Author: ronaldo
 *
 * Created on 14 de setembro de 2023, 09:41
 */

#ifndef _CLOSERCOOPERATORAGENTINDEXERALGORITHM_H
#define _CLOSERCOOPERATORAGENTINDEXERALGORITHM_H

class _token;
class _agent;
class _stepSite;
class _closerCooperatorAgentIndexerAlgorithm {
public:
    
    _closerCooperatorAgentIndexerAlgorithm(
            unsigned min_step_distance = 1,
            unsigned min_endpoint_distance = 1,
            unsigned max_step_distance = 1,
            unsigned max_endpoint_distance = 1);
    
    _closerCooperatorAgentIndexerAlgorithm(const _closerCooperatorAgentIndexerAlgorithm&);
   
    virtual ~_closerCooperatorAgentIndexerAlgorithm();
    
    virtual void solve(
        const _token&, 
        const _stepSite&,
        const _stepSite&,
        std::vector<std::pair<int, unsigned>>& vpair_agentid_epdist) const;
    
    void setMax_endpoint_distance(unsigned max_endpoint_distance) {
        this->max_endpoint_distance = max_endpoint_distance;
    }

    void setMax_step_distance(unsigned max_step_distance) {
        this->max_step_distance = max_step_distance;
    }

    void setMin_endpoint_distance(unsigned min_endpoint_distance) {
        this->min_endpoint_distance = min_endpoint_distance;
    }

    void setMin_step_distance(unsigned min_step_distance) {
        this->min_step_distance = min_step_distance;
    }

    
private:
    
    unsigned 
            max_step_distance, 
            max_endpoint_distance, 
            min_step_distance, 
            min_endpoint_distance;
    
};

#endif /* _CLOSERCOOPERATORAGENTINDEXERALGORITHM_H */

