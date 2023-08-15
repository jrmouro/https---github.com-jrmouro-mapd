/* 
 * File:   PathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:35
 */

#ifndef PATHALGORITHM_H
#define PATHALGORITHM_H

#include "IntegerMap.h"
#include "_stepPath.h"

class PathAlgorithm {
public:
    virtual bool solve(const IntegerMap& map, const _site& start, const _site& goal, _stepPath& path, unsigned step, int type) const = 0;
};

#endif /* PATHALGORITHM_H */

