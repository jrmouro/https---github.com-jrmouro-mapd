
/* 
 * File:   _env_map.h
 * Author: ronaldo
 *
 * Created on 21 de dezembro de 2023, 04:36
 */

#ifndef _ENV_MAP_H
#define _ENV_MAP_H

#include <functional>
#include "Recoverable.h"

class _stepSite;
class _stepPath;

class _env_map : public Recoverable{
    
public:
    
    virtual void listNeighborFreePaths(
            const _stepSite& site, 
            int type, 
            const std::function<bool(const _stepSite&)>& function) const = 0;
    
    virtual bool isPathDefinitelyFree(const _stepSite& site, int type) const = 0;
    
    virtual void resetTypesInStepColunm(unsigned row, unsigned column, int type) = 0;
    
    virtual void setMoving(const _stepPath& path, int type) = 0;
    
    virtual void deleteMoving(const _stepPath& path, int type) = 0;
    
    virtual unsigned getColumn_size() const = 0;

    virtual unsigned getRow_size() const = 0;
    
    virtual unsigned getStep_size() const = 0;
    
    virtual void stepView(const _stepPath& path)const = 0;    
    virtual void stepView(unsigned from, unsigned to)const = 0;        
    virtual void stepView(unsigned step)const = 0;    
    virtual void free_agent_view()const = 0;    
    virtual void max_step_view()const = 0;
    
    virtual void save() = 0;
    virtual void unsave() = 0;
    virtual void restore() = 0;
    
};

#endif /* _ENV_MAP_H */

