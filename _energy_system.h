/* 
 * File:   _energy_system.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 10:40
 */

#ifndef _ENERGY_SYSTEM_H
#define _ENERGY_SYSTEM_H

#include <cmath>
#include "_energy_regime.h"
#include "_energy_charge.h"

template <class ID, class UNIT>
class _energy_system : Identifiable<std::string>{
    
public:
    
    _energy_system(
            std::string id,
            const _energy_charge<UNIT>& energy_charge,  
            const _energy_regime<ID, UNIT>& regime) :
                        _id(id),
                        current_level(std::min<UNIT>(energy_charge.current_level, energy_charge.maximum_level)), 
                        energy_charge(energy_charge), 
                        regime(regime) {}
        
    _energy_system(const _energy_system<ID, UNIT>& other) :
            _id(other._id),
            current_level(other.current_level), 
            energy_charge(other.energy_charge),
            regime(other.regime) { }
    
    virtual std::string id() const {
        return _id + "(" + regime.id() + ")";
    }
    
    void expend(ID id, UNIT& value) {   
                
        bool flag = regime.get(id, value);
        
        if(flag){
            
            current_level -= value;     
            
            if(current_level > energy_charge.maximum_level){
                
                value += (current_level - energy_charge.maximum_level);
                
            }
            
            current_level = std::min<UNIT>(current_level, energy_charge.maximum_level);
            
        } else {
            try {
                std::ostringstream stream;
                stream << "regime type not found";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
        }
    }
           
    UNIT currentLevel() const {
        return current_level;
    }
    
    UNIT criticalLevel() const {
        return energy_charge.critical_level;
    }
    
    UNIT chargedLevel() const {
        return energy_charge.charged_level;
    }
    
    UNIT maximumLevel() const {
        return energy_charge.maximum_level;
    }
    
    bool isAtCriticalLevel()const{
        return energy_charge.critical_level > current_level;
    }
    
    bool isAtChargedLevel()const{
        return energy_charge.charged_level < current_level;
    }
    
    bool isMaximumLevel()const{
        return energy_charge.maximum_level == current_level;
    }
    
    virtual bool isAtDeadLevel() const = 0;
    
    
    
protected:
    const std::string _id;
    UNIT current_level;   
    const _energy_charge<UNIT>& energy_charge;
    const _energy_regime<ID,UNIT>& regime;
    
};

#endif /* _ENERGY_SYSTEM_H */

