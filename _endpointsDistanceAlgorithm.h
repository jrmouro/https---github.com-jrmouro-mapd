/* 
 * File:   _endPointsDistanceAlgorithm.h
 * Author: ronaldo
 *
 * Created on 18 de agosto de 2023, 09:14
 */

#ifndef _ENDPOINTSDISTANCEALGORITHM_H
#define _ENDPOINTSDISTANCEALGORITHM_H

#include <cmath>
#include <climits>
#include <vector>
#include <unordered_map>
#include "_distanceAlgorithm.h"
#include "_astarDistanceAlgorithm.h"

class _map;
class _endpointsDistanceAlgorithm : public _distanceAlgorithm{
    
public:
    
    _endpointsDistanceAlgorithm();
    
    _endpointsDistanceAlgorithm(const _map& map);
    
    _endpointsDistanceAlgorithm(const _endpointsDistanceAlgorithm& other);
    
    virtual ~_endpointsDistanceAlgorithm();
    
    virtual unsigned solve(const _site& start, const _site& goal) const;
    
    void reset(const _map& map);
    
    friend std::ostream& operator<<(std::ostream& os, const _endpointsDistanceAlgorithm& obj) {
        
        os << " - distances matrix - " << std::endl;
        int c = 0;
        for (int i = 0; i < obj.size; i++) {
            c++;
            os << obj.distances[i] << "\t";
            if(c == obj.endpointsSize){
                c = 0;
                os << std::endl;
            }
        }

        return os;
    }

    
private:
    
    unsigned size = 0, mapColunmSize = 0, endpointsSize = 0;
    unsigned *distances = nullptr;
    std::unordered_map<unsigned, unsigned> domain;

};

#endif /* _ENDPOINTSDISTANCEALGORITHM_H */

