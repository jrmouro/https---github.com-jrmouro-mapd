/* 
 * File:   InstanceMap.h
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:33
 */

#ifndef INSTANCEMAP_H
#define INSTANCEMAP_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <map>

#include "SiteMap.h"
#include "EndPoint.h"

class InstanceMap : public Drawable{
public:

    InstanceMap(
        unsigned row_size, 
        unsigned column_size,
            unsigned num_bots,
            unsigned num_endpoints,
            unsigned num_steps) : 
        siteMap(new SiteMap(row_size, column_size)),
        num_bots(num_bots),
        num_endpoints(num_endpoints),
        num_steps(num_steps){ }

    InstanceMap(const InstanceMap& orig) : 
        siteMap(new SiteMap(*orig.siteMap)),
        num_bots(orig.num_bots),
        num_endpoints(orig.num_endpoints),
        num_steps(orig.num_steps){ }

    virtual ~InstanceMap() {
    
        delete this->siteMap;
        
    }
    
    static InstanceMap* load(std::string filename_instance) {

        std::ifstream filestream(filename_instance);

        if (filestream.is_open()) {

            std::string line;
            getline(filestream, line);
            std::stringstream ss(line);

            unsigned row, col, bots, steps, endpoints;

            ss >> row;
            ss >> col;
            
            ss.clear();
            getline(filestream, line);
            ss << line;
            ss >> endpoints;
            
            ss.clear();
            getline(filestream, line);
            ss << line;
            ss >> bots;
            
            ss.clear();
            getline(filestream, line);
            ss << line;
            ss >> steps;

            auto imap = new InstanceMap(row, col, bots, endpoints, steps);            

            imap->loadMap(filestream);

            return imap;

        }

        std::cout << "Instance map file not found." << std::endl;

        return nullptr;

    }
    
    void loadMap(std::ifstream& filestream){
        
        unsigned i = 0, j = 0;
        this->siteMap->load(filestream, [&i, &j, this](unsigned row, unsigned colunm, Site::Type siteType){
             
            if(siteType == Site::Type::endpoint) 
                
                this->endpointMap.insert(std::pair<unsigned, EndPoint>(i++, EndPoint(i, row, colunm)));
            
            else if(siteType == Site::Type::bot) 
                
                this->botMap.insert(std::pair<unsigned, Site>(j++, Site(j, row, colunm, siteType)));
                
            
            
            return false;
        });
        
    }
    
//    void draw(sf::RenderWindow& window){
//        this->siteMap->draw(window);
//    }
    
    virtual void draw(const Render& render) const{
        this->siteMap->draw(render);
    }
    
    unsigned getColumn_size() const {
        return this->siteMap->getColumn_size();
    }

    unsigned getRow_size() const {
        return this->siteMap->getRow_size();
    }
    
    SiteMap* getSiteMap() const {
        return siteMap;
    }
    
    unsigned getNumBots() const {
        return num_bots;
    }

    unsigned getNumEndpoints() const {
        return num_endpoints;
    }

    unsigned getNumSteps() const {
        return num_steps;
    }

    const EndPoint* getEndPointById(unsigned endpointId){
        
        std::map<unsigned, EndPoint>::const_iterator it;
        it = this->endpointMap.find(endpointId);

        if (it != this->endpointMap.end()) {
            
            return &(*it).second;
            
        }
        
        try {
            std::ostringstream stream;
            stream << "invalid endpoint id [" << endpointId << "]";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
        return nullptr;
        
    }
    
    friend std::ostream& operator<<(std::ostream& os, const InstanceMap& obj) {
        os << "num_endpoints: " << obj.num_endpoints << std::endl;
        os << "num_bots: " << obj.num_bots << std::endl;
        os << "num_steps: " << obj.num_steps << std::endl;
        
        os << "endpointMap:" << std::endl;
        std::map<unsigned, EndPoint>::const_iterator it;
        for (it = obj.endpointMap.begin(); it != obj.endpointMap.end(); ++it)
            os << (*it).first << " " << (*it).second << std::endl;   
        
        os << "siteMap:" << std::endl;
        os << *obj.siteMap;
                
        return os;
    }

    
private:
    
    unsigned num_bots, num_endpoints, num_steps;
    SiteMap* siteMap;
    std::map<unsigned,EndPoint> endpointMap;
    std::map<unsigned,Site> botMap;

};

#endif /* INSTANCEMAP_H */

