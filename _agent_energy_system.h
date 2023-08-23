/* 
 * File:   _agent_energy_system.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 14:34
 */

#ifndef _AGENT_ENERGY_SYSTEM_H
#define _AGENT_ENERGY_SYSTEM_H

#include "_energy_system.h"

typedef
enum _agent_energy_regime : int{
    
    charging = -10, // repondo energia
    unloaded = 1, // parado descarregado
    moving = 3, // movendo descarregado
    loaded = 2, // parado carregado
    carrying = 10 // movendo carregado
    
} AER;

class _agent_energy_system : public _energy_system<AER, int>{
public:
    
    _agent_energy_system(int amount) : _energy_system<AER, int>(amount) { 
    
        addRegime(AER::charging, AER::charging);
        addRegime(AER::unloaded, AER::unloaded);
        addRegime(AER::moving, AER::moving);
        addRegime(AER::loaded, AER::loaded);
        addRegime(AER::carrying, AER::carrying);
    
    }
    
    _agent_energy_system(const _agent_energy_system& other):_energy_system<AER, int>(other){}
    
    virtual ~_agent_energy_system(){}
    
    friend std::ostream& operator<<(std::ostream& os, const _agent_energy_system& obj) {
        os << "amount: " << obj.getAmount();
        return os;
    }


};

#endif /* _AGENT_ENERGY_SYSTEM_H */

