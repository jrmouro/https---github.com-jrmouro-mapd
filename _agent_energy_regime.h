/* 
 * File:   _agent_energy_regime.h
 * Author: ronaldo
 *
 * Created on 19 de setembro de 2023, 14:43
 */

#ifndef _AGENT_ENERGY_REGIME_H
#define _AGENT_ENERGY_REGIME_H

#include "_energy_regime.h"

typedef
enum _agent_energy_regime_type : int{
    
    charging, // repondo energia
    unloaded, // parado descarregado
    moving, // movendo descarregado
    loaded, // parado carregado
    carrying // movendo carregado
    
} AERT;

class _agent_energy_regime : public _energy_regime<AERT, int> {
    
public:
    
    _agent_energy_regime(
        std::string id,
        int charging,
        int unloaded,
        int moving,
        int loaded,
        int carrying) : _energy_regime<AERT, int>(id){
        
        this->add(AERT::charging, charging);
        this->add(AERT::unloaded, unloaded);
        this->add(AERT::moving, moving);
        this->add(AERT::loaded, loaded);
        this->add(AERT::carrying, carrying);
        
    }
        
    _agent_energy_regime(const _agent_energy_regime& orig):_energy_regime<AERT, int>(orig){}
    
    virtual ~_agent_energy_regime(){}

};

#endif /* _AGENT_ENERGY_REGIME_H */

