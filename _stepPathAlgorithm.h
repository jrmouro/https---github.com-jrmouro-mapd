/* 
 * File:   _stepPathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:35
 */

#ifndef _STEPPATHALGORITHM_H
#define _STEPPATHALGORITHM_H

#include "_stepMap.h"
#include "_stepPath.h"

class _stepPathAlgorithm {
public:
    virtual bool solve(const _stepMap& map, const _site& start, const _site& goal, _stepPath& path, unsigned step, int type) const = 0;
};

#endif /* _STEPPATHALGORITHM_H */

