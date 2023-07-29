
/* 
 * File:   PathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:35
 */

#ifndef PATHALGORITHM_H
#define PATHALGORITHM_H

#include "Path.h"

class PathAlgorithm {
public:
    virtual bool solve(const SiteMap& map, const Site& start, const Site& goal, Path& path) const = 0;
};

#endif /* PATHALGORITHM_H */

