/* 
 * File:   _endpointsPathAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 19 de novembro de 2023, 06:37
 */

#include "_endpointsPathAlgorithm.h"

_endpointsPathAlgorithm::_endpointsPathAlgorithm() :
    _endpointsDistanceAlgorithm() { }

//_endpointsPathAlgorithm::_endpointsPathAlgorithm(const _map& map) :
//    _endpointsDistanceAlgorithm(map) { }

_endpointsPathAlgorithm::_endpointsPathAlgorithm(const _endpointsPathAlgorithm& other) :
    _endpointsDistanceAlgorithm(other) {

    if(size > 0){

        paths.resize(size);

        for(unsigned i= 0; i < size; i++)

                paths[i] = other.paths[i];

    }

}

_endpointsPathAlgorithm::~_endpointsPathAlgorithm(){}

void _endpointsPathAlgorithm::reset(const _map& map){
    
    _astarAlgorithm astar;

    if(distances != nullptr) delete distances;
    domain.clear();

    endpointsSize = map.getNumEndpoints();
    size = endpointsSize * endpointsSize;
    
    if(size > 0){
        
        paths.resize(size);
    
        mapColunmSize = map.getColumn_size();

        distances = new unsigned[size];
        
        _path path;

        for(unsigned i= 0; i < size; i++){
            distances[i] = UINT_MAX;
            paths[i] = path;
        }

        unsigned index = 0;

        map.listEndpoints([&index, this](const _site& endpoint){

            domain.insert(std::pair<unsigned,unsigned>(
                endpoint.GetRow() * mapColunmSize + endpoint.GetColunm(), 
                index++));

            return false;

        });

        map.listEndpoints(

        [&astar, &map, this](const _site& e1){

            auto it1 = domain.find(e1.GetRow() * mapColunmSize + e1.GetColunm());

            if(it1 != domain.end()){

                unsigned row = it1->second;

                map.listEndpoints([&map, &astar, &e1, &row, this](const _site& e2){

                    auto it2 = domain.find(e2.GetRow() * mapColunmSize + e2.GetColunm());

                    if(it2 != domain.end()){

                        unsigned colunm = it2->second;

                        unsigned l1 = row * endpointsSize + colunm;
                        unsigned l2 = colunm * endpointsSize + row;

                        if(e1.match(e2)){

                            distances[l1] = distances[l2] = 0;
                            paths[l1] = paths[l2] = _path(e1);

                        } else {

                            if(distances[l1] == UINT_MAX){
                                
                                bool flag = astar.solve(map, e1, e2, paths[l1]);
                                
                                if(flag){

                                    distances[l1] = distances[l2] = paths[l1].size() - 1;
                                    
                                    paths[l1].getReverse(paths[l2]);
                                
                                } else {
                                    
                                    try {
                                        std::ostringstream stream;
                                        stream << "path not found from " << e1 << " to " << e2;
                                        MAPD_EXCEPTION(stream.str());
                                    } catch (std::exception& e) {
                                        std::cout << e.what() << std::endl;
                                        std::abort();
                                    }
                                    
                                }

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

                    return false;

                });

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

            return false;

        });
    
    }
    
}

const _path& _endpointsPathAlgorithm::solve_path(const _site& start, const _site& goal) const{
    
    if(distances == nullptr){

        try {
            std::ostringstream stream;
            stream << "paths do not initialized";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }


    }
    
    auto it1 = domain.find(start.GetRow() * mapColunmSize + start.GetColunm());

    if(it1 == domain.end()){

        try {
            std::ostringstream stream;
            stream << "start site domain error: " << goal;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }
    
    
    auto it2 = domain.find(goal.GetRow() * mapColunmSize + goal.GetColunm());

    if(it2 == domain.end()){

        try {
            std::ostringstream stream;
            stream << "goal site domain error: " << goal;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }
    
    unsigned row = it1->second, colunm = it2->second;

    return paths[row * endpointsSize + colunm];
    
}