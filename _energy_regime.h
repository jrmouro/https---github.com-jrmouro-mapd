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


template <class ID, class UNIT>
class _energy_regime {
public:
    
    _energy_regime() {}

    _energy_regime(std::map<ID, UNIT> map) : map(map) {}
    
    _energy_regime(const _energy_regime<ID, UNIT>& other) : map(other.map) {}

    UNIT get(ID id)const{
        
        typename std::map<ID,UNIT>::const_iterator it = map.find(id);
        
        if(it != map.end()) {
            
            return it->second;
            
        } 
                    
        try {
            std::ostringstream stream;
            stream << "regime not found";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
              
    }
    
    void add(ID id, UNIT value){
        
        map.insert(std::pair<ID,UNIT>(id, value));
        
    }

    virtual ~_energy_regime(){}
    
private:
    std::map<ID,UNIT> map;
    
};

#endif /* _ENERGY_REGIME_H */

