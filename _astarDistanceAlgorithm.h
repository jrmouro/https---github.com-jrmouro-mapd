/* 
 * File:   _astarDistanceAlgorithm.h
 * Author: ronaldo
 *
 * Created on 18 de agosto de 2023, 00:09
 */

#ifndef _ASTARDISTANCEALGORITHM_H
#define _ASTARDISTANCEALGORITHM_H


#include "_distanceAlgorithm.h"
#include "_map.h"
#include "_path.h"
#include "_astarAlgorithm.h"

class _map;
class _astarDistanceAlgorithm : public _distanceAlgorithm{
public:
    
    _astarDistanceAlgorithm(const _map& map);
    
    _astarDistanceAlgorithm(const _astarDistanceAlgorithm& other);
    
    virtual ~_astarDistanceAlgorithm();
    
    virtual unsigned solve_distance(const _site& start, const _site& goal) const ; 
    
private:
    
    const _map& map;
    
};

#endif /* _ASTARDISTANCEALGORITHM_H */

