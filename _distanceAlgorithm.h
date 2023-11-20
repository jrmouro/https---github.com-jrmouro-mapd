/* 
 * File:   _distanceAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de agosto de 2023, 11:21
 */

#ifndef _DISTANCEALGORITHM_H
#define _DISTANCEALGORITHM_H

class _site;
class _distanceAlgorithm {
public:
    virtual unsigned solve_distance(const _site& start, const _site& goal) const = 0;
};

#endif /* _DISTANCEALGORITHM_H */

