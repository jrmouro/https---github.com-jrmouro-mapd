/* 
 * File:   _agent_energy_system.h
 * Author: ronaldo
 *
 * Created on 23 de agosto de 2023, 14:34
 */

#ifndef _AGENT_ENERGY_SYSTEM_H
#define _AGENT_ENERGY_SYSTEM_H

#include "_energy_system.h"
#include "_agent_energy_regime.h"
#include "_stepPath.h"
#include "_task.h"
#include "_map.h"



typedef
enum _agent_energy_state : int{
    
    dead,
    critical,
    normal,
    charged,
    none
    
} AES;

class _agent_energy_system : public _energy_system<AERT, int>{
    
public:
    
    _agent_energy_system(
            const std::string id,
            const _energy_charge<int>& energy_charge,
            const _agent_energy_regime& agent_energy_regime) :
                _energy_system<AERT, int>(
                        id,
                        energy_charge,
                        agent_energy_regime), 
                charging(energy_charge.current_level) { }
        
    _agent_energy_system(
        const _agent_energy_system& other):
        _energy_system<AERT, int>(other), 
        charging(other.charging){}
    
    virtual ~_agent_energy_system(){}
    
    friend std::ostream& operator<<(std::ostream& os, const _agent_energy_system& obj) {
        os <<  "regime: " << obj.regime << std::endl;
        os << "current level: " << obj.currentLevel() << std::endl;
        os << "critical level: " << obj.criticalLevel() << std::endl;
        os << "charged level: " << obj.chargedLevel() << std::endl;
        os << "maximum level: " << obj.maximumLevel() << std::endl;
        os << "charging: " << obj.charging << std::endl;
        return os;
    }
    
    int getCarringRegime()const{
        int ret = 0;
        regime.get(AERT::carrying, ret);
        return ret;
    }
    
    virtual bool isAtDeadLevel() const{
        return 0 > this->current_level;
    }
    
    virtual AES energyState() const {        
        if(isAtDeadLevel()) return AES::dead;
        if(this->isAtCriticalLevel()) return AES::critical;
        if(this->isAtChargedLevel()) return AES::charged;
        return AES::normal;        
    }    
    
    virtual int appraiseTaskPath(const _map& map, const _task& task, const _stepPath& path) const {
        
        int ret = 0;
        int moving_value;
        regime.get(AERT::moving, moving_value);
        int loaded_value;
        regime.get(AERT::loaded, loaded_value);
        int unloaded_value;
        regime.get(AERT::unloaded, unloaded_value);
        int carrying_value;
        regime.get(AERT::carrying, carrying_value);
        int charging_value;
        regime.get(AERT::charging, charging_value);
        
        bool pickup_flag = true, delivery_flag = true;
        
        path.movingList([map, task, moving_value, loaded_value, unloaded_value, carrying_value, charging_value, &pickup_flag, &delivery_flag, &ret, this](const _stepSite& orig, const _stepSite& dest){
                                   
            if(pickup_flag && orig.match(task.getPickup())){
                
                pickup_flag = false;
                                
            } 
            
            if(!pickup_flag && delivery_flag && orig.match(task.getDelivery())){
                
                delivery_flag = false;
                
            }
                        
            if(map.getTypeOfSite(orig.GetRow(), orig.GetColunm()) == _map::TypeOfSite::bot){
                
                int diff = current_level - charging_value - energy_charge.maximum_level;
                
                if(diff < 0){
                
                    ret += charging_value;
                
                } else {
                    
                    ret += (charging_value + diff);
                    
                }
                
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
        int moving_value;
        regime.get(AERT::moving, moving_value);
        int unloaded_value;
        regime.get(AERT::unloaded, unloaded_value);
        int charging_value;
        regime.get(AERT::charging, charging_value);
        
        path.movingList([map, charging_value, moving_value, unloaded_value, &ret, this](const _stepSite& orig, const _stepSite& dest){
            
            if(map.getTypeOfSite(orig.GetRow(), orig.GetColunm()) == _map::TypeOfSite::bot){
                
                int diff = current_level - charging_value - energy_charge.maximum_level;
                
                if(diff < 0){
                
                    ret += charging_value;
                
                } else {
                    
                    ret += (charging_value + diff);
                    
                }
                
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
    
    void expendStepping(bool isChargingSite, bool stayedSamePlace, bool carrying){
                   
        int value;
        
        if(isChargingSite){

            expend(AERT::charging, value);
            charging -= value;

        }         

        if(stayedSamePlace){
            
            if(carrying){
                
                expend(AERT::loaded, value);
                
            } else {
                
                expend(AERT::unloaded, value);
                
            }

        } else {
            
            if(carrying){
                
                expend(AERT::carrying, value);
                
            } else {
                
                expend(AERT::moving, value);
                
            }

        }
               
    }
   
    
    void expendCarryngStepping(bool isChargingSite, const _stepSite& orig, const _stepSite dest){
                   
        int value;
        
        if(isChargingSite){

            expend(AERT::charging, value);
            charging -= value;

        }         

        if(orig.match(dest)){

            expend(AERT::loaded, value);

        } else {

            expend(AERT::carrying, value);

        }
               
    }
    
    void expendNoCarryngStepping(bool isChargingSite, const _stepSite& orig, const _stepSite dest){
                          
        int value;
        
        if(isChargingSite){

            expend(AERT::charging, value);
            charging -= value;

        }         

        if(orig.match(dest)){

            expend(AERT::unloaded, value);

        } else {

            expend(AERT::moving, value);

        }
               
    }
    
    int getCharging() const {
        return charging;
    }
    
private:
    
    int charging = 0;

};

#endif /* _AGENT_ENERGY_SYSTEM_H */

