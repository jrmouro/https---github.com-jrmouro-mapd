/* 
 * File:   _stepSite.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:25
 */

#ifndef _STEPSITE_H
#define _STEPSITE_H

#include <iostream>
#include "_site.h"


class _stepSite : public _site{
    
public:
    
    _stepSite() : _stepSite(0,0,0){ }

    _stepSite(unsigned step, unsigned row, unsigned colunm) :
    _site(row, colunm), step(step) {}
    
    _stepSite(const _stepSite& other) : _site(other), step(other.step) { }

    virtual ~_stepSite(){}    
    
    unsigned GetStep() const {
        return step;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const _stepSite& obj) {
        os << obj.step << " . " << (_site&)obj;
        return os;
    }
    
protected:
    
    unsigned step;
    
};

#endif /* _STEPSITE_H */

