/* 
 * File:   _thresholdAlgorithm.h
 * Author: ronaldo
 *
 * Created on 21 de agosto de 2023, 06:36
 */

#ifndef _THRESHOLDALGORITHM_H
#define _THRESHOLDALGORITHM_H

#include "_stepPath.h"
#include "_task.h"


class _thresholdAlgorithm {
public:
    _thresholdAlgorithm(const _endpointsPathAlgorithm* endpointspathAlgorithm) :
    endpointsPathAlgorithm(endpointspathAlgorithm) {}
    
    _thresholdAlgorithm(const _thresholdAlgorithm& other) :
    endpointsPathAlgorithm(other.endpointsPathAlgorithm) {}

    virtual ~_thresholdAlgorithm(){}
    
    virtual float solve(const _stepPath& path) const {        
                
        return solve(path.currentSite(), path.goalSite(), path.size() - 1);
        
    } 
    
    virtual float solve(const _stepPath& path, unsigned midlerStep) const {  
        
        if(path.size() > 1){
        
            _stepSite midler = path.get(midlerStep);

            return ((float)endpointsPathAlgorithm->solve_distance(path.currentSite(), midler) + 
                    (float)endpointsPathAlgorithm->solve_distance(midler, path.goalSite())) / (path.size() - 1);
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "invalid path size( < 2): " << path.size();
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
        }
        
        return .0f;
        
    } 
    
    virtual float solve(const _task& task, unsigned size) const {        
                
        return solve(task.getPickup(), task.getDelivery(), size);
        
    } 
    
    virtual float solve(const _site& s1, const _site& s2, unsigned size) const {
                
        return (float)endpointsPathAlgorithm->solve_distance(s1, s2) / (float)size;
        
    }
    
    virtual bool solve(const _site& s1, const _site& s2, unsigned size, float threshold) const {
        
        return threshold <= solve(s1, s2, size);
        
    }
    
    virtual bool solve(const _stepPath& path, float threshold) const {
        
        return threshold <= solve(path);
        
    }
    
    virtual bool solve(const _stepPath& path, unsigned midlerStep, float threshold) const {
        
        return threshold <= solve(path, midlerStep);
        
    }
    
private:
    const _endpointsPathAlgorithm* endpointsPathAlgorithm;
};

#endif /* _THRESHOLDALGORITHM_H */

