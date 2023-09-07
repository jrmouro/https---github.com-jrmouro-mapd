/* 
 * File:   _conflictEndpointTaskDeliveryAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 08:39
 */

#ifndef _CONFLICTENDPOINTTASKDELIVERYALGORITHM_H
#define _CONFLICTENDPOINTTASKDELIVERYALGORITHM_H

#include "_token.h"
#include "_site.h"
#include "_task.h"

class _conflictEndpointTaskDeliveryAlgorithm {
public:
    
    virtual bool solve(
            const _token& token, 
            const _site& site, 
            _task& conflitTask) const {
        
        bool conflit = false;

        token.listPendingTasks([site, &conflit, &conflitTask, this](const _task & task) {

            if (task.getDelivery().match(site)) {

                conflit = true;

                conflitTask = task;

                return true;

            }

            return false;

        });

        return conflit;
        
    }
    
};

#endif /* _CONFLICTENDPOINTTASKDELIVERYALGORITHM_H */

