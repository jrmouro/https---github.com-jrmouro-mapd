/* 
 * File:   _allocation.h
 * Author: ronaldo
 *
 * Created on 25 de outubro de 2023, 11:25
 */

#ifndef _ALLOCATION_H
#define _ALLOCATION_H

#include <functional>

class _ga_token;
class _ga_agent;
class _task;

class _allocation {
public:
    virtual _allocation* clone() const = 0;
    virtual bool isValid()const = 0;
    virtual void nextPlanningUpdate(
        const _ga_token&,
        const std::function<bool(const _ga_agent*, const _task*)>&) = 0;
};

#endif /* _ALLOCATION_H */

