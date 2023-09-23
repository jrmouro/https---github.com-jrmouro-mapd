/* 
 * File:   _map.h
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:33
 */

#ifndef _MAP_H
#define _MAP_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <set>
#include <SFML/Graphics/Color.hpp>
#include "Drawable.h"
#include "Render.h"
#include "_site.h"
#include "MapdException.h"
#include "_endpointsDistanceAlgorithm.h"

class _endpointsDistanceAlgorithm;
class _map : public Drawable{
public:
    
    enum TypeOfSite {
        none,
        path,
        bot,
        endpoint,        
        task
    };
    
    static const class _TypeColorMap{
        
    public:
        
        _TypeColorMap() {
            map.insert(std::pair<TypeOfSite,sf::Color>(TypeOfSite::bot, sf::Color::Yellow));
            map.insert(std::pair<TypeOfSite,sf::Color>(TypeOfSite::endpoint, sf::Color::Blue));
            map.insert(std::pair<TypeOfSite,sf::Color>(TypeOfSite::path, sf::Color::Green));
            map.insert(std::pair<TypeOfSite,sf::Color>(TypeOfSite::task, sf::Color::Cyan));
            map.insert(std::pair<TypeOfSite,sf::Color>(TypeOfSite::none, sf::Color::Red));
        }
        
        sf::Color get(TypeOfSite siteType) const {
            std::unordered_map<TypeOfSite,sf::Color>::const_iterator it;
            it = map.find(siteType);
            if(it != map.end()) 
                return it->second;            
            return sf::Color::Transparent;
        }

    private:
        
        std::unordered_map<TypeOfSite,sf::Color> map;
        
    } TypeColorMap;
        
    _map() : _map(0, 0){}

    _map(unsigned row_size, unsigned colunm_size) :  colunm_size(colunm_size), row_size(row_size){

        unsigned size = row_size * colunm_size;
        
        if(size > 0){
        
            sites = new TypeOfSite[size];
        
        }

    }

    _map(const _map& other);
    
    virtual ~_map() {

        if (sites != nullptr) delete [] sites;
        if (endpointsDistanceAlgorithm != nullptr) delete endpointsDistanceAlgorithm;

    }
    
    void setTypeOfSite(unsigned linearLocation, TypeOfSite value) {
        
        if (linearLocation < row_size * colunm_size)
            this->sites[linearLocation] = value;

    }

    void setTypeOfSite(unsigned row, unsigned column, TypeOfSite value) {

        if (row < row_size && column < colunm_size)
            this->sites[row * colunm_size + column] = value;

    }
    
    void resetTaskTypeOfSite() {

        for (unsigned r = 0; r < row_size; r++) {

            for (unsigned c = 0; c < colunm_size; c++) {

                auto p = sites[r * colunm_size + c];

                if (p == TypeOfSite::task) {
                    
                    sites[r * colunm_size + c] = TypeOfSite::endpoint;
                    
                } 

            }

        }

    }

    TypeOfSite getTypeOfSite(int row, int colunm) const {

        if (row > -1 && row < row_size && colunm > -1 && colunm < colunm_size)
            return this->sites[row * colunm_size + colunm];

        return TypeOfSite::none;

    }
    
    TypeOfSite getTypeOfSite(const _site& site) const {

        return getTypeOfSite(site.GetRow(), site.GetColunm());

    }
       
    friend std::ostream& operator<<(std::ostream& os, const _map& obj) {
        
        os << "row_size: " << obj.row_size << std::endl;
        os << "colunm_size: " << obj.colunm_size << std::endl;
        
        for (unsigned r = 0; r < obj.row_size; r++) {

            for (unsigned c = 0; c < obj.colunm_size; c++) {

                auto p = obj.sites[r * obj.colunm_size + c];

                if (p == TypeOfSite::none) {
                    os << "@";
                } else if (p == TypeOfSite::bot) {
                    os << "r";
                } else if (p == TypeOfSite::endpoint) {
                    os << "e";
                }else if (p == TypeOfSite::task) {
                    os << "t";
                } else {
                    os << " ";
                }

            }

            os << std::endl;

        }

        os << std::endl;     

        return os;

    }

    void load(std::ifstream& filestream, std::function<bool(unsigned, unsigned, TypeOfSite)> func);

    unsigned getColumn_size() const {
        return colunm_size;
    }

    unsigned getRow_size() const {
        return row_size;
    }
    
    unsigned getNumBots() const {
        return botMap.size();
    }

    unsigned getNumEndpoints() const {
        return endpoints.size();
    }
    
    virtual bool isNodeBelonging(const _site& site) const {
        
        return site.GetRow() < row_size && site.GetColunm() < colunm_size;
        
    }
    
    virtual void listNeighborFreeSites(const _site& site, const std::function<bool(const _site&)>& function) const {
        
        if(this->isNodeBelonging(site)){
        
            site.listNeighbors([this, &function](const _site& neigh){

                if(this->isNodeBelonging(neigh)){ 
                
                    unsigned index = neigh.GetRow() * colunm_size + neigh.GetColunm();

                    if(this->sites[index] != TypeOfSite::none) 
                        if(function(neigh)) return true;

                } 

                return false;
                
            });
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "invalid site";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
                
    }
            
    virtual void draw(const Render& render) const;
    
    const _site* getNoBotEndPointById(unsigned endpointId) const {
        
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
    
    void listEndpoints(const std::function<bool(const _site&)>& function) const {
        
        for (auto endpoint : endpoints) {
            
            if(function(endpoint)) return;

        }
        
    }
    
    const _endpointsDistanceAlgorithm& getEndpointsDistanceAlgorithm() const;
       
private:

    TypeOfSite* sites = nullptr;
    
    unsigned 
            colunm_size = 0, 
            row_size = 0;
    
    std::vector<_site> endpoints;
    std::map<unsigned,_site> endpointMap;
    std::map<unsigned,_site> botMap;
    
    _endpointsDistanceAlgorithm* endpointsDistanceAlgorithm = nullptr;

};

#endif /* _MAP_H */

