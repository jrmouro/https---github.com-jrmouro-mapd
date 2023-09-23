/* 
 * File:   _energy_charge.h
 * Author: ronaldo
 *
 * Created on 19 de setembro de 2023, 16:15
 */

#ifndef _ENERGY_CHARGE_H
#define _ENERGY_CHARGE_H

template<class UNIT>
class _energy_charge {
public:
    _energy_charge(
            const UNIT current_level, 
            const UNIT maximum_level, 
            const UNIT charged_level, 
            const UNIT critical_level) :
                    current_level(current_level), 
                    maximum_level(maximum_level), 
                    charged_level(charged_level), 
                    critical_level(critical_level) { }
                    
    _energy_charge(
            const _energy_charge<UNIT>& other) :
                    current_level(other.current_level), 
                    maximum_level(other.maximum_level), 
                    charged_level(other.charged_level), 
                    critical_level(other.critical_level) { }


    const UNIT current_level, maximum_level, charged_level, critical_level;
    
};

#endif /* _ENERGY_CHARGE_H */

