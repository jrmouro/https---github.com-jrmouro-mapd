/* 
 * File:   StepSite.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:25
 */

#ifndef STEPSITE_H
#define STEPSITE_H

#include <cmath>
#include "Site.h"
#include "MapdException.h"

class StepSite : public Site{
public:

    StepSite() : _step(0), Site(0, 0) { }
    
    StepSite(unsigned step, unsigned row, unsigned colunm) :
    _step(step), Site(row, colunm) { }

    StepSite(const StepSite& orig) :
    _step(orig._step), Site(orig) {}

    virtual ~StepSite() { }

    unsigned step() const {
        return _step;
    }

    friend std::ostream& operator<<(std::ostream& os, const StepSite& obj) {
        os << obj._step << " . " << obj.step() << " . " << obj.colunm();
        return os;
    }

private:
    
    unsigned _step;

};

#endif /* STEPSITE_H */

