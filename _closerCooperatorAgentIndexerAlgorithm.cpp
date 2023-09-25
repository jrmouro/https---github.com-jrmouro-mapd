/* 
 * File:   _closerCooperatorAgentIndexerAlgorithm.cpp
 * Author: ronaldo
 * 
 * Created on 14 de setembro de 2023, 09:41
 */
#include <vector>
#include "_closerCooperatorAgentIndexerAlgorithm.h"
#include "_token.h"
#include "_agent.h"
#include "_stepSite.h"
#include "_closerEndpointIndexerAlgorithm.h"
#include "_manhattanAlgorithm.h"

_closerCooperatorAgentIndexerAlgorithm::_closerCooperatorAgentIndexerAlgorithm(
        unsigned min_step_distance,
        unsigned min_endpoint_distance,
        unsigned max_distance, 
        unsigned max_endpoint_distance) :
min_step_distance(min_step_distance),
min_endpoint_distance(min_endpoint_distance),
max_step_distance(max_distance),
max_endpoint_distance(max_endpoint_distance) {}

_closerCooperatorAgentIndexerAlgorithm::_closerCooperatorAgentIndexerAlgorithm(
        const _closerCooperatorAgentIndexerAlgorithm& other) :
min_step_distance(other.min_step_distance),
min_endpoint_distance(other.min_endpoint_distance),
max_step_distance(other.max_step_distance),
max_endpoint_distance(other.max_endpoint_distance) {}

_closerCooperatorAgentIndexerAlgorithm::~_closerCooperatorAgentIndexerAlgorithm() {}

void _closerCooperatorAgentIndexerAlgorithm::solve(
        const _token& token, 
        const _stepSite& pickup,
        const _stepSite& delivery, 
        std::vector<std::pair<int, unsigned>>& vpair_agentid_epdist) const {

    _manhattanAlgorithm manhattanAlgorithm;

    token.listConstAgents([&token, pickup, delivery, manhattanAlgorithm, &vpair_agentid_epdist, this](const _agent& agent) {

        if(agent.isFree() && !agent.isCharging() && !agent.isAtEnergyCriticalLevel()) {
        
            const unsigned pickup_delivery_site_dist = manhattanAlgorithm.solve(delivery, pickup);
            const unsigned agent_delivery_site_dist = manhattanAlgorithm.solve(delivery, agent.currentSite());

            if(agent_delivery_site_dist <= pickup_delivery_site_dist){

                const unsigned pickup_step_dist = agent.currentSite().step_distance(pickup);
                const unsigned pickup_site_dist = token.getEndpointsDistanceAlgorithm().solve(pickup, agent.currentSite());

                if (pickup_step_dist >= min_step_distance && 
                    pickup_site_dist >= min_endpoint_distance && 
                    pickup_step_dist <= max_step_distance && 
                    pickup_site_dist <= max_endpoint_distance) {

                    std::vector<std::pair<int, unsigned>>::iterator it = vpair_agentid_epdist.begin();

                    for (; it != vpair_agentid_epdist.end(); it++) {

                        if (pickup_site_dist < it->second) {

                            vpair_agentid_epdist.insert(it, std::make_pair(agent.id(), pickup_site_dist));

                            return false;

                        }

                    }

                    vpair_agentid_epdist.push_back(std::make_pair(agent.id(), pickup_site_dist));            


                }

            }
        
        }

        return false;
        
    });       

}