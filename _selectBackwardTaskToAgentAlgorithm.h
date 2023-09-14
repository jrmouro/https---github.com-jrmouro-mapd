/* 
 * File:   _bacwardTaskDeliveryEndpointAlgorithm.h
 * Author: ronaldo
 *
 * Created on 12 de setembro de 2023, 10:00
 */

#ifndef _BACKWARDTASKDELIVERYENDPOINTALGORITHM_H
#define _BACKWARDTASKDELIVERYENDPOINTALGORITHM_H

#include <functional>
#include "_token.h"
#include "_site.h"
#include "_stepPath.h"

class _backwardTaskDeliveryEndpointAlgorithm {
    
public:
    
    _backwardTaskDeliveryEndpointAlgorithm(float delivery_threshold) :
    delivery_threshold(delivery_threshold) { }
    
    _backwardTaskDeliveryEndpointAlgorithm(const _backwardTaskDeliveryEndpointAlgorithm& other) :
    delivery_threshold(other.delivery_threshold) { }

    virtual bool solve(const _token& token, const _stepSite& pickup, const _stepPath& taskPath, _site& endpoint) const {
        
        _thresholdAlgorithm thresholdAlgorithm(token.getEndpointsDistanceAlgorithm());
        
        bool ret = false;
        
        taskPath.backward([&ret, token, pickup, &endpoint](const _stepSite& site){
            
            if(site.step_match(pickup)) return true;
            
            if(token.isTaskDeliveryEndpoint(site)){
                
                unsigned step = site.GetStep() - pickup.GetStep();

                ret = thresholdAlgorithm.solve(pickup, site, step, delivery_threshold);

                if (ret) {

                    endpoint = site;

                    return true;

                }
                    
            }
                            
            return false;
            
        });
        
        return ret;
        
    }
    
private:
    
    float   delivery_threshold = .0f;
    
};

#endif /* _BACKWARDTASKDELIVERYENDPOINTALGORITHM_H */

