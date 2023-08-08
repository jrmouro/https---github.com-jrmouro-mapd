/* 
 * File:   PathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:35
 */

#ifndef PATHALGORITHM_H
#define PATHALGORITHM_H

#include "BinaryMap.h"
#include "BinaryPath.h"

class PathAlgorithm {
public:
    virtual bool solve(const BinaryMap& map, const _site& start, const _site& goal, BinaryPath& path, unsigned step) const = 0;
};

#endif /* PATHALGORITHM_H */

