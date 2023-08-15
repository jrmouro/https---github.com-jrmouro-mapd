/* 
 * File:   _agent_state.cpp
 * Author: ronaldo
 * 
 * Created on 14 de agosto de 2023, 10:39
 */

#include "_agent_state.h"
#include "Circle.h"
#include "Text.h"
#include "Render.h"
#include "_agent.h"
#include "_agent_free.h"
#include "_agent_occupied.h"

void _agent_state::onUpdatePath(_token& token) const {}

void _agent_state::onMoveUpdate(_token& token) const {}

void _agent_state::onDraw(const Render& render) const {}

_agent_state* _agent_state::builder(AgentStateId stateId, _agent* agent) const {

    switch (stateId) {
        case AgentStateId::free: return new _agent_free(agent);
        case AgentStateId::occupied: return new _agent_occupied(agent);
    }

    return nullptr;

}