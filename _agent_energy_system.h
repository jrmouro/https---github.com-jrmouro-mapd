/* 
 * File:   _agent_energy_system.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 14:34
 */

#ifndef _AGENT_ENERGY_SYSTEM_H
#define _AGENT_ENERGY_SYSTEM_H

#include "_energy_system.h"
#include "_stepPath.h"
#include "_task.h"
#include "_map.h"

typedef
enum _agent_energy_regime : int{
    
    charging = -10, // repondo energia
    unloaded = 1, // parado descarregado
    moving = 3, // movendo descarregado
    loaded = 2, // parado carregado
    carrying = 10 // movendo carregado
    
} AER;

typedef
enum _agent_energy_state : int{
    
    dead,
    critical,
    normal,
    charged,
    none
    
} AES;

class _agent_energy_system : public _energy_system<AER, int>{
    
public:
    
    _agent_energy_system(int current_level, const int maximum_level, const int energyChargingLevel, const int critical_level) :
    _energy_system<AER, int>(current_level, maximum_level, energyChargingLevel, critical_level), charging(current_level) {
        
        addRegime(AER::charging, AER::charging);
        addRegime(AER::unloaded, AER::unloaded);
        addRegime(AER::moving, AER::moving);
        addRegime(AER::loaded, AER::loaded);
        addRegime(AER::carrying, AER::carrying);
        
    }
        
    _agent_energy_system(const _agent_energy_system& other):_energy_system<AER, int>(other), charging(other.current_level){}
    
    virtual ~_agent_energy_system(){}
    
    friend std::ostream& operator<<(std::ostream& os, const _agent_energy_system& obj) {
        os << "current level: " << obj.currentLevel() << " / charging: " << obj.charging;
        return os;
    }
    
    virtual bool isAtDeadLevel() const{
        return 0 >= this->current_level;
    }
    
    virtual AES energyState() const {        
        if(isAtDeadLevel()) return AES::dead;
        if(this->isAtCriticalLevel()) return AES::critical;
        if(this->isAtChargedLevel()) return AES::charged;
        return AES::normal;        
    }   
    
    virtual AES energyState(int estimative) const {        
        if(0 >= this->current_level - estimative) return AES::dead;
        if(this->critical_level >= this->current_level - estimative) return AES::critical;
        if(this->charged_level <= this->current_level - estimative) return AES::charged;
        return AES::normal;        
    }   
    
    virtual int appraiseTaskPath(const _map& map, const _task& task, const _stepPath& path) const {
        
        int ret = 0;
        int moving_value = regime.get(AER::moving);
        int loaded_value = regime.get(AER::loaded);
        int unloaded_value = regime.get(AER::unloaded);
        int carrying_value = regime.get(AER::carrying);
        int charging_value = regime.get(AER::charging);
        
        bool pickup_flag = true, delivery_flag = true;
        
        path.moveList([map, task, moving_value, loaded_value, unloaded_value, carrying_value, charging_value, &pickup_flag, &delivery_flag, &ret](const _stepSite& orig, const _stepSite& dest){
                                   
            if(pickup_flag && orig.match(task.getPickup())){
                
                pickup_flag = false;
                                
            } 
            
            if(!pickup_flag && delivery_flag && orig.match(task.getDelivery())){
                
                delivery_flag = false;
                
            }
                        
            if(map.getType(orig.GetRow(), orig.GetColunm()) == _map::Type::bot){
                
                ret += charging_value;
                
            } 
            
            if(pickup_flag){
                    
                if(orig.match(dest)){

                    ret += unloaded_value;

                } else {

                    ret += moving_value;

                }

            } else {

                if(delivery_flag){ // carrying

                    if(orig.match(dest)){

                        ret += loaded_value;

                    } else {

                        ret += carrying_value;

                    }

                } else { // moving

                    if(orig.match(dest)){

                        ret += unloaded_value;

                    } else {

                        ret += moving_value;

                    }

                }                  
                  
            }
            
            return false;
            
        });
        
        return ret;
        
    }
    
    virtual int appraiseNoCarryngPath(const _map& map, const _stepPath& path) const {
        
        int ret = 0;
        int moving_value = regime.get(AER::moving);
        int unloaded_value = regime.get(AER::unloaded);
        int charging_value = regime.get(AER::charging);
        
        path.moveList([map, charging_value, moving_value, unloaded_value, &ret](const _stepSite& orig, const _stepSite& dest){
            
            if(map.getType(orig.GetRow(), orig.GetColunm()) == _map::Type::bot){
                
                ret += charging_value;
                
            } 
                
            if(orig.match(dest)){

                ret += unloaded_value;

            } else {

                ret += moving_value;

            }
            
            return false;
            
        });
                
        return ret;
        
    }
    
    virtual int appraiseNoCarryngStepping(const _map& map, const _stepSite& orig, const _stepSite dest) const {
        
        int ret = 0;
        int moving_value = regime.get(AER::moving);
        int unloaded_value = regime.get(AER::unloaded);
        int charging_value = regime.get(AER::charging);
                            
        if(map.getType(orig.GetRow(), orig.GetColunm()) == _map::Type::bot){

            ret += charging_value;

        }         

        if(orig.match(dest)){

            ret += unloaded_value;

        } else {

            ret += moving_value;

        }
               
        return ret;
        
    }
    
    virtual int appraiseCarryngStepping(const _map& map, const _stepSite& orig, const _stepSite dest) const {
        
        int ret = 0;
        int carrying_value = regime.get(AER::carrying);
        int loaded_value = regime.get(AER::loaded);
        int charging_value = regime.get(AER::charging);
                            
        if(map.getType(orig.GetRow(), orig.GetColunm()) == _map::Type::bot){

            ret += charging_value;

        }         

        if(orig.match(dest)){

            ret += loaded_value;

        } else {

            ret += carrying_value;

        }
               
        return ret;
        
    }
    
    void expendCarryngStepping(bool isChargingSite, const _stepSite& orig, const _stepSite dest){
                                    
        if(isChargingSite){

            expend(AER::charging);
            charging -= AER::charging;

        }         

        if(orig.match(dest)){

            expend(AER::loaded);

        } else {

            expend(AER::carrying);

        }
               
    }
    
    void expendNoCarryngStepping(bool isChargingSite, const _stepSite& orig, const _stepSite dest){
                                    
        if(isChargingSite){

            expend(AER::charging);
            charging -= AER::charging;

        }         

        if(orig.match(dest)){

            expend(AER::unloaded);

        } else {

            expend(AER::moving);

        }
               
    }
    
    int getCharging() const {
        return charging;
    }
    
private:
    
    int charging = 0;

};

#endif /* _AGENT_ENERGY_SYSTEM_H */

