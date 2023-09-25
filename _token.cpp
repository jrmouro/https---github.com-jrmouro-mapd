#include "_token.h"
#include "_agent.h"
#include "_thresholdAlgorithm.h"
#include "_updateTokenAlgorithms.h"

void _token::error_site_collision_check() const {

    unsigned count1 = agents.size();

    for (auto pagent1 : agents) {

        unsigned count2 = count1;

        for (auto pagent2 : agents) {

            if (count2 > 0) {

                if (pagent1.second.id() != pagent2.second.id() &&
                        pagent1.second.currentSite().match(pagent2.second.currentSite())) {

                    try {
                        std::ostringstream stream;
                        stream << "site collision between agent " << pagent1.second.id();
                        stream << " and agent " << pagent2.second.id() << " step(" << currentStep << ")";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

                count2--;

            } else {

                break;

            }

        }

        count1--;

    }

}

void _token::error_edge_collision_check() const {

    unsigned count1 = agents.size();

    for (auto pagent1 : agents) {

        unsigned count2 = count1;

        for (auto pagent2 : agents) {

            if (count2 > 0) {

                if (pagent1.second.id() != pagent2.second.id() &&
                        pagent1.second.previousSite().match(pagent2.second.currentSite()) &&
                        pagent2.second.previousSite().match(pagent1.second.currentSite())) {

                    try {
                        std::ostringstream stream;
                        stream << "edge collision between agent " << pagent1.second.id();
                        stream << " and agent " << pagent2.second.id() << " step(" << currentStep << ")";
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

                count2--;

            } else {

                break;

            }

        }

        count1--;

    }

}

void _token::setMoving(const _agent& agent, const _stepPath& path) {

    stepMap.setMoving(path, agent.id());

}

int _token::currentEnergy() const {
    int ret = 0;
    listConstAgents([&ret](const _agent& agent) {
        ret += agent.currentEnergyLevel();
        return false;
    });
    return ret;
}

int _token::energyExpenditure() const {
    int ret = 0;
    listConstAgents([&ret](const _agent& agent) {
        ret += (agent.energyCharging() - agent.currentEnergyLevel());
        return false;
    });
    return ret;
}

void _token::listAgents(const std::function<bool(_agent&) > function) {

    for (auto& pagent : agents) {

        if (function(pagent.second)) return;

    }
    
}

void _token::listConstAgents(const std::function<bool(const _agent&) > function) const {

    for (auto& pagent : agents) {

        if (function(pagent.second)) return;

    }
    
    


}

void _token::draw(const Render& render) const {

    for (auto& pagent : agents) {

        pagent.second.draw(render);

    }

}

_token::TokenUpdateType _token::updateTrivialPathToAgent(_agent& agent) {

    auto uta = _updateTokenAlgorithms::getInstance();

    TokenUpdateType ret = TokenUpdateType::none;

    bool flag = uta->getUpdateTrivialPathToAgentAlgorithm().solve(*this, agent);

    if (flag) {

        ret = TokenUpdateType::trivial;

    } else {

        try {
            std::ostringstream stream;
            stream << "trivial path error " << std::endl << agent;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

    return ret;

}

//_token::TokenUpdateType _token::updateChargingTrivialPathToAgent(_agent& agent) {
//
//    auto uta = _updateTokenAlgorithms::getInstance();
//
//    TokenUpdateType ret = TokenUpdateType::none;
//
//    bool flag = uta->getUpdateTrivialPathToAgentAlgorithm().solve(*this, agent);
//
//    if (flag) {
//
//        ret = TokenUpdateType::charging_trivial;
//
//    } else {
//
//        try {
//            std::ostringstream stream;
//            stream << "charging trivial path error " << std::endl << agent;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }
//
//    return ret;
//
//}