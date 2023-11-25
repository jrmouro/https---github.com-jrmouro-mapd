/* 
 * File:   _stepPath.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 13:58
 */

#ifndef _STEPPATH_H
#define _STEPPATH_H

#include <climits>
#include <vector>
#include <functional>
#include "MapdException.h"
#include "_path.h"


class _stepSite;
class _stepPath{
public:
    
    _stepPath();
    
    _stepPath(const _stepSite& site);
    
    _stepPath(const _stepPath& orig);
    
    _stepPath& operator=(const _stepPath& right);
    
    virtual ~_stepPath();

    void add(const _stepSite& site);
    
    void progress(const _stepSite& site);
    
    void progress(const _stepPath& path);
        
    const _stepSite& goalSite()const;
    
    const _stepSite& currentSite() const;
    
    const _stepSite& futureSite() const;
    
    _stepSite get(unsigned step) const;
    
    _stepSite pop();
    
    unsigned size() const;

    friend std::ostream& operator<<(std::ostream& os, const _stepPath& obj);

    void clear();
    
    bool empty() const;
        
    void backward(const std::function<bool(const _stepSite&)>& function, unsigned begin = 0) const;
    
    void forward(const std::function<bool(const _stepSite&)>& function, unsigned begin = 0) const;
    
    void movingList(const std::function<bool(const _stepSite&, const _stepSite&)>& function, unsigned begin = 0) const;
    
    static bool trivialSteppingPath(_stepPath& path);
    
    bool isTrivial()const;
    
    unsigned stepToArrive(const _site& goal) const;
    
    static void buildFromPath(unsigned initialStep, const _path& path, _stepPath& stepPath);
    
    bool collideWith(const _stepPath& other) const;
    
    bool collideSiteWith(const _stepPath& other) const;
    
    unsigned collideSiteCount(const _stepPath& other) const;
       
        
private:
        
    std::vector<_stepSite> sites;
    
};

#endif /* _STEPPATH_H */

