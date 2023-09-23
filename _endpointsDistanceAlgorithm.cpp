#include "_endpointsDistanceAlgorithm.h"

_endpointsDistanceAlgorithm::_endpointsDistanceAlgorithm(){}
    
_endpointsDistanceAlgorithm::_endpointsDistanceAlgorithm(const _map& map){
    reset(map);
}

_endpointsDistanceAlgorithm::_endpointsDistanceAlgorithm(const _endpointsDistanceAlgorithm& other) :
        size(other.size), 
        mapColunmSize(other.mapColunmSize), 
        endpointsSize(other.endpointsSize),  
        domain(other.domain){

    if(size > 0){
        
        distances = new unsigned[size];

        for(unsigned i= 0; i < size; i++)
            distances[i] = other.distances[i];
    
    }

}

_endpointsDistanceAlgorithm::~_endpointsDistanceAlgorithm(){

    if(distances != nullptr) delete [] distances;

}

unsigned _endpointsDistanceAlgorithm::solve(const _site& start, const _site& goal) const{
        
    if(distances != nullptr){

        auto it1 = domain.find(start.GetRow() * mapColunmSize + start.GetColunm());

        if(it1 != domain.end()){

            auto it2 = domain.find(goal.GetRow() * mapColunmSize + goal.GetColunm());

            if(it2 != domain.end()){

                unsigned row = it1->second, colunm = it2->second;

                return distances[row * endpointsSize + colunm];

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

void _endpointsDistanceAlgorithm::reset(const _map& map){

    _astarDistanceAlgorithm astar(map);

    if(distances != nullptr) delete distances;
    domain.clear();

    endpointsSize = map.getNumEndpoints();
    size = endpointsSize * endpointsSize;
    mapColunmSize = map.getColumn_size();

    distances = new unsigned[size];

    for(unsigned i= 0; i < size; i++)
        distances[i] = UINT_MAX;

    unsigned index = 0;

    map.listEndpoints([&index, this](const _site& endpoint){

        domain.insert(std::pair<unsigned,unsigned>(
            endpoint.GetRow() * mapColunmSize + endpoint.GetColunm(), 
            index++));

        return false;

    });

    map.listEndpoints(
            
    [astar, map,this](const _site& e1){

        auto it1 = domain.find(e1.GetRow() * mapColunmSize + e1.GetColunm());

        if(it1 != domain.end()){

            unsigned row = it1->second;

            map.listEndpoints([astar, e1, row, this](const _site& e2){

                auto it2 = domain.find(e2.GetRow() * mapColunmSize + e2.GetColunm());

                if(it2 != domain.end()){

                    unsigned colunm = it2->second;

                    unsigned l1 = row * endpointsSize + colunm;
                    unsigned l2 = colunm * endpointsSize + row;

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

