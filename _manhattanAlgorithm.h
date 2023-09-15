/* 
 * File:   _manhattanAlgorithm.h
 * Author: ronaldo
 *
 * Created on 6 de agosto de 2023, 11:23
 */

#ifndef _MANHATTANALGORITHM_H
#define _MANHATTANALGORITHM_H

#include "_distanceAlgorithm.h"

class _manhattanAlgorithm : public _distanceAlgorithm{
public:
    virtual unsigned solve(const _site& start, const _site& goal) const {
        return std::abs((int)start.GetRow() - (int)goal.GetRow()) + 
                std::abs((int)start.GetColunm() - (int)goal.GetColunm());        
    }
};

#endif /* _MANHATTANALGORITHM_H */

