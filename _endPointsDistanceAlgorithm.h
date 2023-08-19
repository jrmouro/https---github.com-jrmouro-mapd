/* 
 * File:   _endPointsDistanceAlgorithm.h
 * Author: ronaldo
 *
 * Created on 18 de agosto de 2023, 09:14
 */

#ifndef _ENDPOINTSDISTANCEALGORITHM_H
#define _ENDPOINTSDISTANCEALGORITHM_H

#include <climits>
#include <vector>
#include <unordered_map>
#include "_distanceAlgorithm.h"
#include "_map.h"
#include "_astarDistanceAlgorithm.h"

class _endpointsDistanceAlgorithm : public _distanceAlgorithm{
    
public:
    
    _endpointsDistanceAlgorithm(){}
    
    _endpointsDistanceAlgorithm(const _map& map, const std::vector<_site>& endpoints){
        reset(map, endpoints);
    }
    
    _endpointsDistanceAlgorithm(const _endpointsDistanceAlgorithm& other) :
    size(other.size), domain(other.domain) {
        
        distances = new unsigned[size];
        
        for(unsigned i= 0; i < size; i++)
            distances[i] = other.distances[i];
        
    }
    
    virtual ~_endpointsDistanceAlgorithm(){
    
        if(distances != nullptr) delete distances;
    
    }
    
    virtual unsigned solve(const _site& start, const _site& goal) const{
        
        if(distances != nullptr){
        
            auto it1 = domain.find(start.GetRow() * colunmSize + start.GetColunm());
            
            if(it1 != domain.end()){
            
                auto it2 = domain.find(goal.GetRow() * colunmSize + goal.GetColunm());
                
                if(it2 != domain.end()){
                    
                    unsigned row = it1->second, colunm = it2->second;
                    
                    return distances[row * colunmSize + colunm];
                    
                } else {
                    
                    try {
                        std::ostringstream stream;
                        stream << "goal site domain error: " << goal;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }
                                        
                }
            
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "start site domain error: " << start;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }
                
            }
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "distances do not initialized";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
            
        }
        
        return UINT_MAX;        
        
    }
    
    void reset(const _map& map, const std::vector<_site>& endpoints){
        
        _astarDistanceAlgorithm astar(map);
        
        if(distances != nullptr) delete distances;
        domain.clear();
        
        size = endpoints.size() * endpoints.size();
        colunmSize = map.getColumn_size();
        
        distances = new unsigned[size];
        
        for(unsigned i= 0; i < size; i++)
            distances[i] = UINT_MAX;
        
        unsigned index = 0;
        for (auto endpoint : endpoints) {
                        
            domain.insert(std::pair<unsigned,unsigned>(
                endpoint.GetRow() * colunmSize + endpoint.GetColunm(), 
                index++));                     
                        
        }
        
        
        for (auto e1 : endpoints) {
                        
            auto it1 = domain.find(e1.GetRow() * colunmSize + e1.GetColunm());
            
            if(it1 != domain.end()){
                
                unsigned row = it1->second;
            
                for (auto e2 : endpoints) {

                    auto it2 = domain.find(e2.GetRow() * colunmSize + e2.GetColunm());
                    
                    if(it2 != domain.end()){
                        
                        unsigned colunm = it2->second;
                                     
                        unsigned l1 = row * colunmSize + colunm;
                        unsigned l2 = colunm * colunmSize + row;

                        if(e1.match(e2)){
                            
                            distances[l1] = distances[l2] = 0;

                        } else {
                            
                            if(distances[l1] == UINT_MAX){
                                
                                distances[l1] = distances[l2] = astar.solve(e1, e2);
                                
                            }

                        }
                    
                    } else {
                        
                        try {
                            std::ostringstream stream;
                            stream << "endpoint domain error: " << e2;
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }
                        
                    }

                }
            
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "endpoint domain error: " << e1;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }
                
            }
            
        }
        
    }
    
private:
    unsigned size = 0, colunmSize = 0;
    unsigned *distances = nullptr;
    std::unordered_map<unsigned, unsigned> domain;

};

#endif /* _ENDPOINTSDISTANCEALGORITHM_H */

