/* 
 * File:   _endpointsPathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 19 de novembro de 2023, 06:37
 */

#ifndef _ENDPOINTSPATHALGORITHM_H
#define _ENDPOINTSPATHALGORITHM_H

#include "_endpointsDistanceAlgorithm.h"

class _endpointsPathAlgorithm : public _endpointsDistanceAlgorithm{
public:
    
    _endpointsPathAlgorithm();
    
//    _endpointsPathAlgorithm(const _map& map);
    
    _endpointsPathAlgorithm(const _endpointsPathAlgorithm& other);

    virtual ~_endpointsPathAlgorithm();
    
    virtual void reset(const _map& map);
    
    virtual const _path& solve_path(const _site& start, const _site& goal) const;
    
private:
    
    std::vector<_path> paths;

};

#endif /* _ENDPOINTSPATHALGORITHM_H */

