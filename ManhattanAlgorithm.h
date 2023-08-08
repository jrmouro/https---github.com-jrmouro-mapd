/* 
 * File:   ManhattanAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de agosto de 2023, 11:23
 */

#ifndef MANHATTANALGORITHM_H
#define MANHATTANALGORITHM_H

#include "DistanceAlgorithm.h"

class ManhattanAlgorithm : public DistanceAlgorithm{
public:
    virtual unsigned solve(const _site& start, const _site& goal) const{
        return std::abs((int)start.GetRow() - (int)goal.GetRow()) + 
                std::abs((int)start.GetColunm() - (int)goal.GetColunm());        
    }
};

#endif /* MANHATTANALGORITHM_H */

