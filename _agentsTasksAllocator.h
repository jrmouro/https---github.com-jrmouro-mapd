/* 
 * File:   _agentsTasksAllocator.h
 * Author: ronaldo
 *
 * Created on 9 de outubro de 2023, 13:27
 */

#ifndef _AGENTSTASKSALLOCATOR_H
#define _AGENTSTASKSALLOCATOR_H

#include "Identifiable.h"
#include <string>

class _ga_token;
class _allocation;

class _agentsTasksAllocator : public Identifiable<std::string>{
public:
    
    _agentsTasksAllocator(const std::string id) :  _id(id) {}
    
    _agentsTasksAllocator(const _agentsTasksAllocator& other) :  _id(other._id) { }

    virtual _allocation* borrow(const _ga_token&)  = 0;
    
    virtual _allocation* restore(const _ga_token&, _allocation*)  = 0;
    
    virtual void giveBack(_allocation*)  = 0;
    virtual _allocation* borrowClone(_allocation*)  = 0;
    
    virtual std::string id() const{
        return _id;
    }
    
    virtual _agentsTasksAllocator* emptyClone() const = 0;
        
private:
    
    std::string _id;
    
};

#endif /* _AGENTSTASKSALLOCATOR_H */

