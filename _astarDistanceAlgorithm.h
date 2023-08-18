/* 
 * File:   _astarDistanceAlgorithm.h
 * Author: ronaldo
 *
 * Created on 18 de agosto de 2023, 00:09
 */

#ifndef _ASTARDISTANCEALGORITHM_H
#define _ASTARDISTANCEALGORITHM_H

#include <climits>
#include "_distanceAlgorithm.h"
#include "SiteMap.h"
#include "_path.h"
#include "_astarAlgorithm.h"

class _astarDistanceAlgorithm : public _distanceAlgorithm{
public:
    
    _astarDistanceAlgorithm(const SiteMap& map) :
    map(map) { }
    
    _astarDistanceAlgorithm(const _astarDistanceAlgorithm& other) :
    map(other.map) { }


    
    virtual ~_astarDistanceAlgorithm(){}
    
    virtual unsigned solve(const _site& start, const _site& goal) const{
        
        if(map.isNodeBelonging(start)){
            
            if(map.isNodeBelonging(goal)){
                
                _astarAlgorithm astar;
                
                _path path;
                
                if(astar.solve(map, start, goal, path))
                    return path.size() - 1;
                
            } else {
                
                
                try {
                    std::ostringstream stream;
                    stream << "invalid site: " << goal;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }
                
            }
            
        }else{
            
            try {
                std::ostringstream stream;
                stream << "invalid site: " << start;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
        return UINT_MAX;
        
    }
    
    
private:
    
    const SiteMap& map;
    
};

#endif /* _ASTARDISTANCEALGORITHM_H */

