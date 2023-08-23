/* 
 * File:   _energy_system.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 10:40
 */

#ifndef _ENERGY_SYSTEM_H
#define _ENERGY_SYSTEM_H

#include "_energy_regime.h"


template <class ID, class UNIT>
class _energy_system {
public:
    
    _energy_system(UNIT amount) : amount(amount) {}
    
    _energy_system(UNIT amount, _energy_regime<ID, UNIT> regime) :
    amount(amount), regime(regime) {}

    _energy_system(const _energy_system<ID, UNIT>& other) :
    amount(other.amount), regime(other.regime) {}
    
    void expend(ID id) {
        amount -= regime.get(id);
    }
    
    void provision(UNIT value){
        amount += value;
    }
    
    void addRegime(ID id, UNIT value){
        regime.add(id, value);
    }
    
    UNIT getAmount() const {
        return amount;
    }
    
private:
    UNIT amount;
    _energy_regime<ID,UNIT> regime;
};

#endif /* _ENERGY_SYSTEM_H */

