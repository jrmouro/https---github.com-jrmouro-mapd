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

void _agent_state::onUpdatePath(_system& system, _agent* agent) const {}

void _agent_state::onMoveUpdate(_system& system,  _agent* agent) const {}

void _agent_state::onDraw(const Render& render, const _agent* const agent) const {}

void _agent_state::changeState(_agent* agent, _agent_state* state) const{
    agent->changeState(state);
}

