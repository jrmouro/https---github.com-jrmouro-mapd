/* 
 * File:   _agent_state.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 10:39
 */

#ifndef _AGENT_STATE_H
#define _AGENT_STATE_H

#include "_task.h"
#include "_stepMap.h"

class _token;
class Render;
class _agent;

class _agent_state {
    
public:
        
    _agent_state(){}    
    _agent_state(const _agent_state& other) {}   
    
    virtual ~_agent_state(){} 
    
    virtual std::string stateName()const = 0;
      
    virtual void changeState(_agent& agent, _agent_state* state) const;
    
    // assigned Charging Path
    virtual void onAssignedChargingPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onAssignedChargingPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onAssignedChargingPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // assigned Trivial Rest
    virtual void onAssignedRestPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onAssignedRestPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onAssignedRestPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // assigned Charging Trivial Path
    virtual void onAssignedChargingTrivialPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onAssignedChargingTrivialPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onAssignedChargingTrivialPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // assigned Trivial Path
    virtual void onAssignedTrivialPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onAssignedTrivialPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onAssignedTrivialPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // assigned Task
    virtual void onAssignedTaskInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onAssignedTaskInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onAssignedTaskInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // unassigned Task
    virtual void onUnassignedTaskInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onUnassignedTaskInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onUnassignedTaskInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // assigned Charging Task
    virtual void onAssignedChargingTaskInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onAssignedChargingTaskInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onAssignedChargingTaskInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // unassigned Charging Task
    virtual void onUnassignedChargingTaskInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onUnassignedChargingTaskInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onUnassignedChargingTaskInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // path
    virtual void onPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // trivial path
    virtual void onTrivialPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onTrivialPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onTrivialPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // task path
    virtual void onDeliveryInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onDeliveryInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onDeliveryInEnergyDeadLevel(_token&,  _agent&) const {}
    virtual void onPickupInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onPickupInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onPickupInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // charging task path
    virtual void onChargingDeliveryInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onChargingDeliveryInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onChargingDeliveryInEnergyDeadLevel(_token&,  _agent&) const {}
    virtual void onChargingPickupInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onChargingPickupInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onChargingPickupInEnergyDeadLevel(_token&,  _agent&) const {}
    virtual void onChargingTaskEndpointInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onChargingTaskEndpointInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onChargingTaskEndpointInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // rest path
    virtual void onFinishedRestPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onFinishedRestPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onFinishedRestPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
    // charging path
    virtual void onFinishedChargingPathInEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onFinishedChargingPathInEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onFinishedChargingPathInEnergyDeadLevel(_token&,  _agent&) const {}
    
        
    virtual void onEnterEnergyCriticalLevel(_token&,  _agent&) const {}
    virtual void onEnterEnergyNormalLevel(_token&,  _agent&) const {}
    virtual void onEnterEnergyDeadLevel(_token&,  _agent&) const {}
        
        
//    virtual void onAssignedTask(_token&,  _agent&) const {}
//    virtual void onUnassignedTask(_token&,  _agent&) const {}
//    
//    virtual void onAssignedChargingPath(_token&,  _agent&) const {}
//    
//    virtual void onAssignedNewRestPath(_token&,  _agent&) const {}
//    
//    virtual void onAssignedTrivialPath(_token&,  _agent&) const {}
    
    
    
    virtual void onUpdatePath(_token&,  _agent&) const;    
    virtual void onBeforeStepping(_token&,  _agent&) const;
    virtual void onEnergyExpend(_token&,  _agent&) const;
    virtual void onStepping(_token&,  _agent&) const;
    virtual void onAfterStepping(_token&,  _agent&) const;
    
    virtual void onDraw(const Render&, const _agent&) const;
                
};

#endif /* _AGENT_STATE_H */

