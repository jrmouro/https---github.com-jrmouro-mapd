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
#include "Writable.h"
#include "_endpointsDistanceAlgorithm.h"

class InstanceMap : public Drawable, public Writable{
public:
    
    friend class InstanceMAPD;
            
    InstanceMap(
        unsigned step_size,
        unsigned row_size, 
        unsigned colunm_size) : 
            map(row_size, colunm_size),
            stepMap(step_size, row_size, colunm_size){ }
            
    InstanceMap(const InstanceMap& orig) : 
        map(orig.map),
        stepMap(orig.stepMap){ }

    virtual ~InstanceMap() { }
    
    virtual void writeHeader(std::ostream& fs) const {
        Writable::strWrite(*this, fs, "step_size", true); 
        Writable::strWrite(*this, fs, "row_size", true);
        Writable::strWrite(*this, fs, "col_size", true);
        Writable::strWrite(*this, fs, "num_bots", true);
        Writable::strWrite(*this, fs, "num_endpoints");
    }   
    
    virtual void writeRow(std::ostream& fs) const {
        Writable::uintWrite(*this, fs, stepMap.getStep_size(), true); 
        Writable::uintWrite(*this, fs, stepMap.getRow_size(), true);
        Writable::uintWrite(*this, fs, stepMap.getColumn_size(), true);
        Writable::uintWrite(*this, fs, map.getNumBots(), true);
        Writable::uintWrite(*this, fs, map.getNumEndpoints());
    }
    
    static InstanceMap* load(std::string filename_instance) {

        std::ifstream filestream(filename_instance);

        if (filestream.is_open()) {

            std::string line;
            getline(filestream, line);

            int row_size, colunm_size, bots, step_size, endpoints;
            
            std::stringstream(line) >> row_size >> colunm_size;
            
            getline(filestream, line);
            std::stringstream(line) >> endpoints;
            
            getline(filestream, line);
            std::stringstream(line) >> bots;
            
            getline(filestream, line);
            std::stringstream(line) >> step_size;

            auto imap = new InstanceMap(step_size, row_size, colunm_size);            

            imap->loadMap(filestream);

            return imap;

        }

        std::cout << "Instance map file not found." << std::endl;

        return nullptr;

    }
    
    void loadMap(std::ifstream& filestream){
        
        unsigned i = 0, j = 0;
        
        this->map.load(filestream, [&i, &j, this](unsigned row, unsigned colunm, _map::TypeOfSite siteType){
            
            if(siteType == _map::TypeOfSite::none){
                
                this->stepMap.resetTypesInStepColunm(row, colunm, _stepMap::NodeType::blockedNode);
                
            } else if(siteType == _map::TypeOfSite::bot){
                
                this->stepMap.resetTypesInStepColunm(row, colunm, j++);
                
            }else{
                
                this->stepMap.resetTypesInStepColunm(row, colunm, _stepMap::NodeType::freeNode);
                
            }
                                    
            return false;
            
        });
        
    }
        
    virtual void draw(const Render& render) const{
        this->map.draw(render);
    }
            
    const _map& getMap() const {
        return map;
    }
    
    const _stepMap& getStepMap() const {
        return stepMap;
    }
    
    void setTaskEndpoint(unsigned row, unsigned colunm) {
        
        this->map.setTypeOfSite(row, colunm, _map::TypeOfSite::task);        
        
    } 
    
    void retsetTaskEndpoint() {
        
        this->map.resetTaskTypeOfSite();        
        
    } 
        
    friend std::ostream& operator<<(std::ostream& os, const InstanceMap& obj) {
               
        os << "siteMap:" << std::endl;
        os << obj.map;
        
        os << "binaryMap:" << std::endl;
        os << obj.stepMap;
                
        return os;
    }
        
private:
    
    _map map;
    _stepMap stepMap;

};

#endif /* INSTANCEMAP_H */

