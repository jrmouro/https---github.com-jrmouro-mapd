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

template <class ID, class UNIT>
class _energy_system {
    
public:
    
    _energy_system(UNIT current_level, const UNIT maximum_level, const UNIT charging_level, const UNIT critical_level, _energy_regime<ID, UNIT> regime) :
    current_level(current_level), critical_level(critical_level), maximum_level(maximum_level), charging_level(charging_level), regime(regime) {}
    
    _energy_system(UNIT current_level, const UNIT maximum_level, const UNIT charging_level, const UNIT critical_level) :
    current_level(current_level), critical_level(critical_level), maximum_level(maximum_level), charging_level(charging_level), regime(regime) {}

    
    _energy_system(const _energy_system<ID, UNIT>& other) :
    current_level(other.current_level), critical_level(other.critical_level), charging_level(other.charging_level), maximum_level(other.maximum_level), regime(other.regime) { }
    
    void expend(ID id) {        
        current_level -= regime.get(id);        
        current_level = std::min<UNIT>(current_level, maximum_level);
    }
        
    void addRegime(ID id, UNIT value){
        regime.add(id, value);
    }
    
    UNIT currentLevel() const {
        return current_level;
    }
    
    bool isAtCriticalLevel()const{
        return critical_level > current_level;
    }
    
    bool isAtChargingLevel()const{
        return charging_level >= current_level;
    }
    
    bool isMaximumLevel()const{
        return maximum_level == current_level;
    }
    
    virtual bool isAtDeadLevel() const = 0;
    
protected:
    
    UNIT current_level;
    const UNIT critical_level, charging_level, maximum_level;    
    _energy_regime<ID,UNIT> regime;
    
};

#endif /* _ENERGY_SYSTEM_H */

