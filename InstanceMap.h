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

#include "_map.h"
#include "_stepMap.h"

class InstanceMap : public Drawable{
public:
    
    friend class InstanceMAPD;
            
    InstanceMap(
        unsigned step_size,
        unsigned row_size, 
        unsigned colunm_size,
        unsigned num_bots,
        unsigned num_endpoints) : 
            map(row_size, colunm_size),
            stepMap(step_size, row_size, colunm_size),
            num_bots(num_bots),
            num_endpoints(num_endpoints){ }
            
    InstanceMap(const InstanceMap& orig) : 
        map(orig.map),
        stepMap(orig.stepMap),
        num_bots(orig.num_bots),
        num_endpoints(orig.num_endpoints){ }

    virtual ~InstanceMap() { }
    
    static InstanceMap* load(std::string filename_instance) {

        std::ifstream filestream(filename_instance);

        if (filestream.is_open()) {

            std::string line;
            getline(filestream, line);
//            std::stringstream ss(line);

            int row_size, colunm_size, bots, step_size, endpoints;

            std::stringstream(line) >> row_size >> colunm_size;
//            ss >> colunm_size;
            
//            ss.clear();
            getline(filestream, line);
//            ss << line;
            std::stringstream(line) >> endpoints;
            
//            ss.clear();
            getline(filestream, line);
//            ss << line;
            std::stringstream(line) >> bots;
            
//            ss.clear();
            getline(filestream, line);
//            ss << line;
            std::stringstream(line) >> step_size;

            auto imap = new InstanceMap(step_size, row_size, colunm_size, bots, endpoints);            

            imap->loadMap(filestream);

            return imap;

        }

        std::cout << "Instance map file not found." << std::endl;

        return nullptr;

    }
    
    void loadMap(std::ifstream& filestream){
        
        unsigned i = 0, j = 0;
        this->map.load(filestream, [&i, &j, this](unsigned row, unsigned colunm, _map::Type siteType){
            
            if(siteType == _map::Type::none){
                
                this->stepMap.setTypesFrom(0, row, colunm, _stepMap::NodeType::blockedNode);
                
            } else if(siteType == _map::Type::bot){
                
                this->stepMap.setTypesFrom(0, row, colunm, j);
                
            }else{
                
                this->stepMap.setTypesFrom(0, row, colunm, _stepMap::NodeType::freeNode);
                
            }
            
                         
            if(siteType == _map::Type::endpoint){
                
                this->endpointMap.insert(std::pair<unsigned, _site>(i++, _site(row, colunm)));
                this->endpoints.push_back(_site(row, colunm));
            
            } else if(siteType == _map::Type::bot) {
                
                this->botMap.insert(std::pair<unsigned, _site>(j++, _site(row, colunm)));    
                this->endpoints.push_back(_site(row, colunm));
            
            }
            
            return false;
            
        });
        
    }
        
    virtual void draw(const Render& render) const{
        this->map.draw(render);
    }
    
    unsigned getColumn_size() const {
        return this->map.getColumn_size();
    }

    unsigned getRow_size() const {
        return this->map.getRow_size();
    }
    
    const _map& getMap() const {
        return map;
    }
    
    const _stepMap& getStepMap() const {
        return stepMap;
    }
    
    void setTaskEndpoint(unsigned row, unsigned colunm) {
        
        this->map.setType(row, colunm, _map::Type::task);        
        
    }
    
    unsigned getNumBots() const {
        return num_bots;
    }

    unsigned getNumEndpoints() const {
        return num_endpoints;
    }

    const _site* getNoBotEndPointById(unsigned endpointId){
        
        std::map<unsigned, _site>::const_iterator it;
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
        
        os << "endpointMap:" << std::endl;
        std::map<unsigned, _site>::const_iterator it;
        for (it = obj.endpointMap.begin(); it != obj.endpointMap.end(); ++it)
            os << (*it).first << " " << (*it).second << std::endl;   
        
        os << "siteMap:" << std::endl;
        os << obj.map;
        
        os << "binaryMap:" << std::endl;
        os << obj.stepMap;
                
        return os;
    }
    
    void listBotsEndPoints(const std::function<bool(unsigned, const _site&)>& function) const {
        
        for (auto elem : botMap) {
            
            if(function(elem.first, elem.second)) return;

        }
        
    }
    
    void listBotsEndPoints(const std::function<bool(const _site&)>& function) const {
        
        for (auto elem : botMap) {
            
            if(function(elem.second)) return;

        }
        
    }    
    
    void listNoBotsEndpoints(const std::function<bool(unsigned, const _site&)>& function)const {
        
        for (auto elem : endpointMap) {
            
            if(function(elem.first, elem.second)) return;

        }
        
    }
    
    void listEndpoints(const std::function<bool(const _site&)>& function){
        
        for (auto endpoint : endpoints) {
            
            if(function(endpoint)) return;

        }
        
    }
    
    const std::vector<_site>& getEndpoints() const {
        return endpoints;
    }


    
private:
    
    unsigned num_bots, num_endpoints;
    _map map;
    _stepMap stepMap;
    std::map<unsigned,_site> endpointMap;
    std::map<unsigned,_site> botMap;
    std::vector<_site> endpoints;

};

#endif /* INSTANCEMAP_H */

