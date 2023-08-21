/* 
 * File:   _fractioningRateAlgorithm.h
 * Author: ronaldo
 *
 * Created on 21 de agosto de 2023, 06:36
 */

#ifndef _FRACTIONINGRATEALGORITHM_H
#define _FRACTIONINGRATEALGORITHM_H

#include "_stepPath.h"


class _fractioningRateAlgorithm {
public:
    _fractioningRateAlgorithm(const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm) :
    endpointsDistanceAlgorithm(endpointsDistanceAlgorithm) {}
    
    _fractioningRateAlgorithm(const _fractioningRateAlgorithm& other) :
    endpointsDistanceAlgorithm(other.endpointsDistanceAlgorithm) {}

    virtual ~_fractioningRateAlgorithm(){}
    
    virtual float solve(const _stepPath& path) const {        
                
        return solve(path.currentSite(), path.goalSite(), path.size());
        
    }    
    
    virtual float solve(const _stepSite& s1, const _stepSite& s2, unsigned size) const {
                
        return (float)endpointsDistanceAlgorithm.solve(s1, s2) / (float)size;
        
    }
    
    virtual bool solve(const _stepSite& s1, const _stepSite& s2, unsigned size, float threshold) const {
        
        return threshold > solve(s1, s2, size);
        
    }
    
    virtual bool solve(const _stepPath& path, float threshold) const {
        
        return threshold > solve(path);
        
    }
    
private:
    const _endpointsDistanceAlgorithm& endpointsDistanceAlgorithm;
};

#endif /* _FRACTIONINGRATEALGORITHM_H */

