/* 
 * File:   DistanceAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de agosto de 2023, 11:21
 */

#ifndef SITEDISTANCEALGORITHM_H
#define SITEDISTANCEALGORITHM_H

class DistanceAlgorithm {
public:
    virtual unsigned solve(const _site& start, const _site& goal) const = 0;
};

#endif /* SITEDISTANCEALGORITHYM_H */

