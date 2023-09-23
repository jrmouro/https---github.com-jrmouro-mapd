/* 
 * File:   _energy_regime.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 11:04
 */

#ifndef _ENERGY_REGIME_H
#define _ENERGY_REGIME_H

#include <map>
#include "MapdException.h"
#include "Identifiable.h"


template <class ID, class UNIT>
class _energy_regime : Identifiable<std::string>{
public:
    
    _energy_regime(std::string id): _id(id){}

    _energy_regime(std::string id, std::map<ID, UNIT> map) : _id(id), map(map) {}
    
    _energy_regime(const _energy_regime<ID, UNIT>& other) : _id(other._id), map(other.map) {}
    
    virtual std::string id() const{
        return _id;
    }

    bool get(ID id, UNIT& value)const{
        
        typename std::map<ID,UNIT>::const_iterator it = map.find(id);
        
        if(it != map.end()) {
            
            value = it->second;
            
            return true;
            
        } 
                    
        try {
            std::ostringstream stream;
            stream << "regime not found";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
        return false;
              
    }
    
    void add(ID id, UNIT value){
        
        map.insert(std::pair<ID,UNIT>(id, value));
        
    }

    virtual ~_energy_regime(){}
    
    
    friend std::ostream& operator<<(std::ostream& os, const _energy_regime<ID, UNIT>& obj) {
        os << "Id: " << obj._id << std::endl;
        return os;
    }

    
private:
    
    const std::string _id;
    std::map<ID,UNIT> map;
    
};

#endif /* _ENERGY_REGIME_H */

