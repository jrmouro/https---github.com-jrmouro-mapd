#include "_astarDistanceAlgorithm.h"
#include "_astarAlgorithm.h"
#include <climits>
#include "_map.h"
#include "_path.h"
#include "_site.h"

_astarDistanceAlgorithm::_astarDistanceAlgorithm(const _map& map) :
    map(map) {}

_astarDistanceAlgorithm::_astarDistanceAlgorithm(const _astarDistanceAlgorithm& other) :
    map(other.map) {}

_astarDistanceAlgorithm::~_astarDistanceAlgorithm() {}

unsigned _astarDistanceAlgorithm::solve(const _site& start, const _site& goal) const{
        
    if(map.isNodeBelonging(start)){

        if(map.isNodeBelonging(goal)){

            _astarAlgorithm astar;

            _path path;

            if(astar.solve(map, start, goal, path))
                return path.size() - 1;

        } else {


            try {
                std::ostringstream stream;
                stream << "invalid goal site: " << goal;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    }else{

        try {
            std::ostringstream stream;
            stream << "invalid start site: " << start;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return UINT_MAX;

}