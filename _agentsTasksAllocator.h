/* 
 * File:   _agentsTasksAllocator.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 13:27
 */

#ifndef _AGENTSTASKSALLOCATOR_H
#define _AGENTSTASKSALLOCATOR_H

class _ga_token;
class _allocation;

class _agentsTasksAllocator {
public:
    virtual _allocation* solve(const _ga_token&) const = 0;
};

#endif /* _AGENTSTASKSALLOCATOR_H */

