/* 
 * File:   _ga_solutionAllocator.h
 * Author: ronaldo
 *
 * Created on 26 de outubro de 2023, 10:07
 */

#ifndef _GA_SOLUTIONALLOCATOR_H
#define _GA_SOLUTIONALLOCATOR_H

#include "_agentsTasksAllocator.h"

class _ga_solutionAllocator : public _agentsTasksAllocator{
public:
    virtual _allocation* solve(const _ga_token&) const;
};

#endif /* _GA_SOLUTIONALLOCATOR_H */

