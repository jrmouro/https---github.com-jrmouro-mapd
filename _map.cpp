#include "_map.h"
#include "Render.h"
#include "Rectangle.h"
#include "_endpointsDistanceAlgorithm.h"

const _map::_TypeColorMap _map::TypeColorMap;

_map::_map(const _map& other) :
    colunm_size(other.colunm_size), 
    row_size(other.row_size), 
    endpoints(other.endpoints), 
    endpointMap(other.endpointMap), 
    botMap(other.botMap){
    
    unsigned size = other.row_size * other.colunm_size;

    if(size > 0){ 

        this->sites = new TypeOfSite[size];

        for (unsigned i = 0; i < size; i++)
            this->sites[i] = other.sites[i];

    }
    
    if(other.endpointsDistanceAlgorithm != nullptr){
        
        endpointsDistanceAlgorithm = new _endpointsDistanceAlgorithm(*(other.endpointsDistanceAlgorithm));
        
    }
    
}

void _map::draw(const Render& render) const {

    for (unsigned r = 0; r < row_size; r++)

        for (unsigned c = 0; c < colunm_size; c++) {

            Rectangle background(

                    sf::Vector2f(
                    c * render.GetCell().first,
                    r * render.GetCell().second),
                    sf::Vector2f(render.GetCell().first, render.GetCell().second),
                    _map::TypeColorMap.get(sites[r * colunm_size + c]));

            background.draw(render);

        }

}

void _map::load(std::ifstream& filestream, std::function<bool(unsigned, unsigned, TypeOfSite)> func) {
                        
    unsigned i = 0, j = 0;

    for (unsigned r = 0; r < this->row_size; r++) {

        std::string line;
        getline(filestream, line);

        for (unsigned c = 0; c < this->colunm_size; c++) {

            TypeOfSite t = TypeOfSite::path;

            if (line[c] == '@') {
                t = TypeOfSite::none;
            } else if (line[c] == 'r') {
                t = TypeOfSite::bot;
                this->endpoints.push_back(_site(r, c));
                this->botMap.insert(std::pair<unsigned, _site>(j++, _site(r, c)));  
            } else if (line[c] == 'e') {
                t = TypeOfSite::endpoint;
                this->endpoints.push_back(_site(r, c));
                this->endpointMap.insert(std::pair<unsigned, _site>(i++, _site(r, c)));
            } 

            this->sites[r * colunm_size + c] = t;

            if(func(r, c, t)) return;

        }

    }

    endpointsDistanceAlgorithm = new _endpointsDistanceAlgorithm(*this);

}

const _endpointsDistanceAlgorithm& _map::getEndpointsDistanceAlgorithm() const {
    return *endpointsDistanceAlgorithm;
}

