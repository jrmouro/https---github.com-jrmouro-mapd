/* 
 * File:   _ga_solution.cpp
 * Author: ronaldo
 * 
 * Created on 17 de outubro de 2023, 09:55
 */
#include "_ga_solution.h"
#include "_endpointsPathAlgorithm.h"
#include "_closerTaskIndexerAlgorithm.h"
#include "_manhattanAlgorithm.h"

_ga_solution::_ga_solution(unsigned validity) : _validity(validity) {
}

_ga_solution::_ga_solution(
        const _ga_token& token,
        unsigned validity) :
_ga_pseudo_solution(),
_validity(validity) {

    this->greedySolution_e(token);

}

_ga_solution::_ga_solution(const _ga_solution& other) :
_ga_pseudo_solution(other),
age(other.age),
_validity(other._validity),
allocation_map(other.allocation_map),
evals(other.evals) {
}

_ga_solution::~_ga_solution() {
}

const std::map<_ga_solution::EvalType, unsigned>& _ga_solution::evaluateAux(const _ga_token& token) {
    
    _manhattanAlgorithm manhattanAlgorithm;

    evals.clear();

    if (allocation_map.empty()) {

        alloc(token);

    }
    
    unsigned makespan = 0;
    double energy = .0;
    std::map<const _ga_agent*, std::vector<_stepSite>> agentSites;
    std::map<const _ga_agent*,std::vector<const _task*>> allocation_copy;
    
    for (auto alloc_pair : allocation_map) {
        
        allocation_copy.insert(std::pair<const _ga_agent*,std::vector<const _task*>>(alloc_pair.first, alloc_pair.second));
        
        auto it = agentSites.insert(std::pair<const _ga_agent*, std::vector<_stepSite>>(alloc_pair.first, std::vector<_stepSite>()));
        
        _stepSite currentSite = it.first->first->currentSite();
        
        it.first->second.push_back(currentSite);
        
        bool flag = true;
        for (auto task : alloc_pair.second) {
            
            unsigned pickupDist = 0, deliveryDist = 0;
            
            if(true){
                
                pickupDist = manhattanAlgorithm.solve_distance(currentSite, task->getPickup());
                flag = false;
                
            } else {
                
                pickupDist = token.getMap().getEndpointsPathAlgorithm()->solve_distance(currentSite, task->getPickup());
                
            }
            
            deliveryDist = token.getMap().getEndpointsPathAlgorithm()->solve_distance(task->getPickup(), task->getDelivery());
            
            currentSite.SetRow(task->getPickup().GetRow());
            currentSite.SetColunm(task->getPickup().GetColunm());
            currentSite.SetStep(currentSite.GetStep() + pickupDist);
            
            it.first->second.push_back(currentSite);
            
            currentSite.SetRow(task->getDelivery().GetRow());
            currentSite.SetColunm(task->getDelivery().GetColunm());
            currentSite.SetStep(currentSite.GetStep() + deliveryDist);
            
            it.first->second.push_back(currentSite);            

        }
        
    }
    
    
    for (auto agentSites_pair1 : agentSites) {
        
        unsigned size = agentSites_pair1.second.size() - 1;
        
        if(size > 0){
            
            double area = agentSites_pair1.second[1].siteStepBoxArea(agentSites_pair1.second[0]);
            
            double energy_cost = (agentSites_pair1.second[1].GetStep() - agentSites_pair1.second[0].GetStep()) *
                                token.getAgent_energy_system().getMovingRegime();
            
            if(area != 0){
            
                for (auto agentSites_pair2 : agentSites) {
                    
                    if(agentSites_pair1.first != agentSites_pair2.first){

                        for (auto stepSite : agentSites_pair2.second) {

                            if(stepSite.insideStepSiteBox(agentSites_pair1.second[0], agentSites_pair1.second[1])){

                                energy += energy_cost / area;

                            }

                        }
                    
                    }
                    
                    

                    if(agentSites_pair2.second.back().GetStep() > makespan){

                        makespan = agentSites_pair2.second.back().GetStep();

                    }

                }      
            
            }
            
        }
        
        for (int i = 1; i < size; i++) {
            
            double area = agentSites_pair1.second[i+1].siteStepBoxArea(agentSites_pair1.second[i]);
            
            double energy_cost = (agentSites_pair1.second[i + 1].GetStep() - agentSites_pair1.second[i].GetStep()) *
                                token.getAgent_energy_system().getMovingRegime();
            
            if(area != 0){
            
                for (auto agentSites_pair2 : agentSites) {
                    
                    if(agentSites_pair1.first != agentSites_pair2.first){

                        for (auto stepSite : agentSites_pair2.second) {

                            if(stepSite.insideStepSiteBox(agentSites_pair1.second[i], agentSites_pair1.second[i+1])){

                                energy += energy_cost / area;

                            }

                        }
                    
                    }

                }  
            
            }
                            
        }

    }
    
    energy *= token.getStepMap().getStep_size();
    
    evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, makespan));
    evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, energy));

    return evals;

}

const std::map<_ga_solution::EvalType, unsigned>& _ga_solution::evaluateAux2(const _ga_token& token) {

    evals.clear();

    if (allocation_map.empty()) {

        alloc(token);

    }

    std::map<_ga_solution::EvalType, unsigned> ret;

    _ga_token tokenAux(token);
    _ga_solution solutionAux(*this);

    unsigned count = 0;

    while (//count++ < validity && 
            tokenAux.getCurrentStep() < tokenAux.getStepMap().getStep_size() &&
            !tokenAux.isIdle()) {

        //            tokenAux.error_site_collision_check(); // TODO retirar
        //            tokenAux.error_edge_collision_check();

        //            if(tokenAux.getCurrentStep() > 300){ // TODO retirar
        //                
        //                try {
        //                    std::ostringstream stream;
        //                    stream << "solution error";
        //                    MAPD_EXCEPTION(stream.str());
        //                } catch (std::exception& e) {
        //                    std::cout << e.what() << std::endl;
        //                    std::abort();
        //                }
        //                
        //            }

        solutionAux.nextPlanningUpdate(tokenAux, [&tokenAux](const _ga_agent* agent, const _task * task) {

            if (task != nullptr) {

                return tokenAux.updateAgentTaskPath(agent->id(), task->id());

            } else {

                return tokenAux.updateAgentTaskPath(agent->id(), INT_MIN);

            }

        });

        tokenAux.stepping();

    }

    evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::makespan, tokenAux.GetMaxPlannedStep()));
    evals.insert(std::pair<_ga_solution::EvalType, unsigned>(_ga_solution::EvalType::energy, tokenAux.energyExpenditure()));

    return evals;

}

//unsigned _ga_solution::evaluate(const _ga_token& token, const _ga_solution::EvalType& evalType) {
//
//    auto evalMap = evaluate(token);
//
//    std::map<EvalType, unsigned>::const_iterator evalTypeIt = evals.find(evalType);
//
//    if (evalTypeIt == evals.end()) {
//
//        try {
//            std::ostringstream stream;
//            stream << "eval type not found: " << evalType;
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//
//    }
//
//    return evalTypeIt->second;
//
//}

//const std::map<_ga_solution::EvalType, unsigned>& _ga_solution::evaluate(const _ga_token& token) {
//
//    if (evals.empty()) {
//
//        return evaluateAux(token);
//
//    }
//
//    return evals;
//
//}

void _ga_solution::disturb(std::default_random_engine& generator) {
    evals.clear();
    allocation_map.clear();
    _ga_pseudo_solution::disturb(generator);
}

void _ga_solution::disturb(unsigned agents_size, unsigned task_size, std::default_random_engine& generator) {
    evals.clear();
    allocation_map.clear();
    _ga_pseudo_solution::disturb(agents_size, task_size, generator);
}

_ga_solution* _ga_solution::get_disturb(std::default_random_engine& generator) const {

    auto ret = new _ga_solution(*this);
    ret->disturb(generator);
    return ret;

}

_ga_solution* _ga_solution::get_disturb(unsigned agents_size, unsigned task_size, std::default_random_engine& generator) const {

    auto ret = new _ga_solution(*this);
    ret->disturb(agents_size, task_size, generator);
    return ret;

}

std::pair<std::pair<_ga_solution*, _ga_solution*>, std::pair<_ga_solution*, _ga_solution*>> _ga_solution::get_crossover(
        const _ga_solution& other,
        unsigned agentsCrossoverPoint,
        unsigned tasksCrossoverPoint) const {

    _ga_solution* childA = new _ga_solution(_validity);
    _ga_solution* childB = new _ga_solution(_validity);
    _ga_solution* childC = new _ga_solution(_validity);
    _ga_solution* childD = new _ga_solution(_validity);

    crossover(*this, other, *childA, *childB, *childC, *childD, agentsCrossoverPoint, tasksCrossoverPoint);

    return std::pair<std::pair<_ga_solution*, _ga_solution*>, std::pair < _ga_solution*, _ga_solution*>>(std::pair<_ga_solution*, _ga_solution*>(childA, childB), std::pair<_ga_solution*, _ga_solution*>(childC, childD));

}

bool _ga_solution::isDominatedBy(const _ga_token& token, _ga_solution& other) {
    
    bool flag = false;
    
    if(this->isEvaluated()){
        
        if(other.isEvaluated()){
            
            for (auto tev : evals) {

                std::map<_ga_solution::EvalType, unsigned>::const_iterator it = other.evals.find(tev.first);

                if (it->second < tev.second) {

                    return false;

                }


                if (tev.second < it->second) {

                    flag = true;

                }


            }  
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "solution is not avaluated: " << other;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    } else {
        
        try {
            std::ostringstream stream;
            stream << "solution is not avaluated: " << *this;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    return flag;

}

_ga_solution* _ga_solution::randon(std::default_random_engine& generator) const {

    auto ret = new _ga_solution(*this);

    ret->disturb(generator);

    return ret;

}

_allocation* _ga_solution::clone() const {
    return new _ga_solution(*this);
}

bool _ga_solution::isValid()const {
    return age < _validity;
}

void _ga_solution::revalidate() {
    age = 0;
}

bool _ga_solution::check(const _ga_token& token)const {

    unsigned numPendingTasks = token.numberOfpendingTasks();
    unsigned numAgents = token.numberOfAgents();

    unsigned count = 0;

    if (numAgents == allocation_map.size()) {

        for (const auto& allocPair : allocation_map) {

            for (auto task : allocPair.second) {

                if (token.getPendingTaskById(task->id()) == nullptr) return true;

                count++;

            }


        }


    }

    return count == numPendingTasks;

}

void _ga_solution::nextPlanningUpdate(
        const _ga_token& token,
        const std::function<bool(const _ga_agent* agent, const _task* task)>& function) {

    if (allocation_map.empty()) {

        alloc(token);

    }

    for (const _ga_agent* agent : agents) {

        std::map<const _ga_agent*, std::vector<const _task*>>::iterator agent_alloc_it = allocation_map.find(agent);

        if (agent_alloc_it != allocation_map.end()) {

            if (agent_alloc_it->second.empty()) {

                function(agent, nullptr);

            } else {

                auto allocTask = agent_alloc_it->second.front();

                if (function(agent, allocTask)) {

                    for (std::vector<const _task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {

                        if ((*it) == allocTask) {

                            (*it) = nullptr;

                            break;

                        }

                    }

                    agent_alloc_it->second.erase(agent_alloc_it->second.begin());

                }

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "allocation error - agent not found: " << *agent;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    }

    age++;

}

_ga_solution& _ga_solution::operator=(const _ga_solution& right) {

    if (this == &right)
        return *this;

    (_ga_pseudo_solution&) (*this) = (_ga_pseudo_solution&) right;

    age = right.age;
    _validity = right._validity;
    evals = right.evals;
    allocation_map = right.allocation_map;

    return *this;

}

std::ostream& operator<<(std::ostream& os, const _ga_solution& obj) {

//    os << (_ga_pseudo_solution&) obj;

    if (obj.isEvaluated()) {

        os << "makespan eval: " << obj.evals.find(_ga_solution::EvalType::makespan)->second << std::endl;
        os << "energy eval: " << obj.evals.find(_ga_solution::EvalType::energy)->second << std::endl;

    } else {

        os << "is not evaluated" << std::endl;

    }
//    if (obj.isAllocated()) {
//
//        os << "allocation:" << std::endl;
//
//        for (auto alloc : obj.allocation_map) {
//
//            os << *alloc.first << ": ";
//
//            for (auto task : alloc.second) {
//
//                os << task->id() << ", ";
//
//            }
//
//            os << std::endl;
//
//        }
//
//    } else {
//
//        os << "is not allocated" << std::endl;
//
//    }

    return os;

}

bool _ga_solution::isEvaluated()const {
    return !evals.empty();
}

bool _ga_solution::isAllocated()const {
    return !allocation_map.empty();
}

void _ga_solution::restore(const _ga_token& token) {

    std::set<const _task*> tasksSet;

    token.listPendingTasks([&tasksSet](unsigned indexTask, const _task & task) {

        tasksSet.insert(&task);

        return false;

    });

    for (std::vector<const _task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {

        tasksSet.erase((*it));

    }

    unsigned agentsIndex = tasks.size() % agents.size();

    for (auto task : tasksSet) {

        if (agentsIndex >= agents.size()) {
            agentsIndex = 0;
        }

        const _ga_agent* agent = agents[agentsIndex++];

        auto agentIt = allocation_map.find(agent);

        if (agentIt != allocation_map.end()) {

            agentIt->second.push_back(task);

        } else {

            try {
                std::ostringstream stream;
                stream << "agent pointer not found: " << agent;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

        tasks.push_back(task);

    }

    evaluateAux(token);

}

void _ga_solution::alloc(const _ga_token& token) {

    allocation_map.clear();

    unsigned agentsIndex = 0;
    unsigned tasksIndex = 0;

    for (const _ga_agent* agent : agents) {

        allocation_map.insert(
                std::pair<const _ga_agent*, std::vector<const _task*>>(
                agent,
                std::vector<const _task*>()));

    }

    while (tasksIndex < tasks.size()) {

        if (agentsIndex >= agents.size()) {
            agentsIndex = 0;
        }

        const _ga_agent* agent = agents[agentsIndex++];

        auto agentIt = allocation_map.find(agent);

        if (agentIt != allocation_map.end()) {

            const _task* task = tasks[tasksIndex];

            if (task != nullptr) {

                agentIt->second.push_back(tasks[tasksIndex]);

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "agent pointer not found: " << agent;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

        tasksIndex++;

    }

}

void _ga_solution::alloc2(const _ga_token& token) {
    
    
    allocation_map.clear();
    
    std::map<const _ga_agent*, _stepSite> agentSites;

    unsigned agentsIndex = 0;
    unsigned tasksIndex = 0;

    for (const _ga_agent* agent : agents) {
                
        agentSites.insert(std::pair<const _ga_agent*, _stepSite>(agent, agent->goalSite()));

        allocation_map.insert(
                std::pair<const _ga_agent*, std::vector<const _task*>>(
                agent,
                std::vector<const _task*>()));

    }

    while (tasksIndex < tasks.size()) {

        if (agentsIndex >= agents.size()) {
            agentsIndex = 0;
        }

        const _ga_agent* agent = agents[agentsIndex++];

        auto agentIt = allocation_map.find(agent);

        if (agentIt != allocation_map.end()) {

            const _task* task = tasks[tasksIndex];

            if (task != nullptr) {

                agentIt->second.push_back(tasks[tasksIndex]);

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "agent pointer not found: " << agent;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

        tasksIndex++;

    }
    
    
    /////////////////////////////////

    allocation_map.clear();
    evals.clear();

    std::map<const _ga_agent*, _stepSite> agentGoalSites;
    const _ga_agent* agent_min = nullptr;
    _stepSite stepSite_min;
    unsigned makespan = 0, energy = 0;

    for (const _ga_agent* agent : agents) {

        _stepSite goal = agent->goalSite();

        if (makespan < goal.GetStep()) {

            makespan = goal.GetStep();

        }

        if (agent_min == nullptr) {

            agent_min = agent;
            stepSite_min = goal;

        }

        if (stepSite_min.GetStep() < goal.GetStep()) {

            agent_min = agent;
            stepSite_min = goal;

        }

        agentGoalSites.insert(std::pair<const _ga_agent*, _stepSite>(agent, goal));

        allocation_map.insert(std::pair<const _ga_agent*, std::vector<const _task*>>(agent, std::vector<const _task*>()));

    }


    if (agent_min != nullptr) {

        for (const _task* task : tasks) {

            unsigned dist = token.getMap().getEndpointsPathAlgorithm()->solve_distance(stepSite_min, task->getPickup());
            energy += dist * token.getAgent_energy_system().getMovingRegime();

            dist = dist + token.getMap().getEndpointsPathAlgorithm()->solve_distance(task->getPickup(), task->getDelivery());

            std::map<const _ga_agent*, _stepSite>::iterator agentsGoalSitesIterator = agentGoalSites.find(agent_min);

            if (agentsGoalSitesIterator != agentGoalSites.end()) {

                agentsGoalSitesIterator->second.SetRow(task->getDelivery().GetRow());
                agentsGoalSitesIterator->second.SetColunm(task->getDelivery().GetColunm());

                unsigned goalMakespan = agentsGoalSitesIterator->second.GetStep() + dist;
                agentsGoalSitesIterator->second.SetStep(goalMakespan);

                if (makespan < goalMakespan) {

                    makespan = goalMakespan;

                }

                std::map<const _ga_agent*, std::vector<const _task*>>::iterator alloc_it = allocation_map.find(agent_min);

                if (alloc_it != allocation_map.end()) {

                    alloc_it->second.push_back(task);

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "agent not found: " << *agent_min;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

                unsigned min = UINT_MAX;

                for (const auto& goal : agentGoalSites) {

                    unsigned step = goal.second.GetStep();

                    if (step < min) {

                        min = step;
                        agent_min = goal.first;
                        stepSite_min = goal.second;

                    }

                }

            } else {

                try {
                    std::ostringstream stream;
                    stream << "agent not found" << *agent_min;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        }

    }

}

//

void _ga_solution::greedySolution(const _ga_token& token) {

    _closerTaskIndexerAlgorithm closerTaskIndexerAlgorithm;

    std::vector<_site> agentsSites(token.numberOfAgents());
    std::map<unsigned, const _task*> pendingTasks;

    token.listPendingTasks([ &pendingTasks](unsigned index, const _task & task) {

        pendingTasks.insert(std::pair<unsigned, const _task*>(task.id(), &task));

        return false;

    });

    this->agents.resize(token.numberOfAgents());

    token.listConstAgents([this, &agentsSites](unsigned index, const _ga_agent & agent) {

        this->agents[index] = &agent;

        this->allocation_map.insert(
                std::pair<const _ga_agent*, std::vector<const _task*>>(
                &agent, std::vector<const _task*>()
                )
                );

        agentsSites[index] = agent.currentSite();

        return false;

    });

    while (!pendingTasks.empty()) {

        for (int i = 0; i < this->agents.size() && !pendingTasks.empty(); i++) {

            std::vector<_task> taskIndex;

            std::map<unsigned, const _task*>::iterator pendingTasks_it = pendingTasks.begin();

            for (; pendingTasks_it != pendingTasks.end(); pendingTasks_it++) {

                closerTaskIndexerAlgorithm.ga_solve(token, *pendingTasks_it->second, agentsSites[i], taskIndex);

            }

            unsigned taskId = taskIndex.front().id();

            pendingTasks_it = pendingTasks.find(taskId);

            if (pendingTasks_it != pendingTasks.end()) {

                std::map<const _ga_agent*, std::vector<const _task*>>::iterator alloc_map_it = this->allocation_map.find(this->agents[i]);

                if (alloc_map_it != this->allocation_map.end()) {

                    alloc_map_it->second.push_back(pendingTasks_it->second);

                    this->tasks.push_back(pendingTasks_it->second);

                    agentsSites[i] = pendingTasks_it->second->getDelivery();

                    pendingTasks.erase(taskId);

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "agent not found" << this->agents[i];
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            } else {

                try {
                    std::ostringstream stream;
                    stream << "task id not found" << taskId;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }


        }

    }

}

void _ga_solution::greedySolution_e(const _ga_token& token) {

    _closerTaskIndexerAlgorithm closerTaskIndexerAlgorithm;

    std::vector<_site> agentsSites(token.numberOfAgents());
    std::map<unsigned, const _task*> pendingTasks;
    std::vector<const _ga_agent*> agentsTemp(token.numberOfAgents(), nullptr);
    std::vector<unsigned> agentsCarryingDistance(token.numberOfAgents(), 0);

    token.listPendingTasks([ &pendingTasks](unsigned index, const _task & task) {

        pendingTasks.insert(std::pair<unsigned, const _task*>(task.id(), &task));

        return false;

    });

    if (token.numberOfpendingTasks() > 0) {

        unsigned r = token.numberOfpendingTasks() % token.numberOfAgents();

        if (r != 0)
            r = token.numberOfAgents() - r;

        tasks.resize(token.numberOfpendingTasks() + r);

        for (int i = 1; i <= r; i++)
            tasks[tasks.size() - i] = nullptr;

    }

    token.listConstAgents([this, &agentsSites, &agentsTemp](unsigned index, const _ga_agent & agent) {

        //        this->agents[index] = &agent;
        agentsTemp[index] = &agent;

        this->allocation_map.insert(
                std::pair<const _ga_agent*, std::vector<const _task*>>(
                &agent, std::vector<const _task*>()
                )
                );

        agentsSites[index] = agent.currentSite();

        return false;

    });

    while (!pendingTasks.empty()) {

        for (int i = 0; i < agentsTemp.size() && !pendingTasks.empty(); i++) {

            std::vector<_task> taskIndex;

            std::map<unsigned, const _task*>::iterator pendingTasks_it = pendingTasks.begin();

            for (; pendingTasks_it != pendingTasks.end(); pendingTasks_it++) {

                closerTaskIndexerAlgorithm.ga_solve(token, *pendingTasks_it->second, agentsSites[i], taskIndex);

            }

            unsigned taskId = taskIndex.front().id();

            pendingTasks_it = pendingTasks.find(taskId);

            if (pendingTasks_it != pendingTasks.end()) {

                std::map<const _ga_agent*, std::vector<const _task*>>::iterator alloc_map_it = this->allocation_map.find(agentsTemp[i]);

                if (alloc_map_it != this->allocation_map.end()) {

                    alloc_map_it->second.push_back(pendingTasks_it->second);

                    //                    this->tasks.push_back(pendingTasks_it->second);

                    agentsSites[i] = pendingTasks_it->second->getDelivery();

                    const unsigned carryingDistance = token.getMap().getEndpointsPathAlgorithm()->solve_distance(
                            pendingTasks_it->second->getPickup(),
                            pendingTasks_it->second->getDelivery());

                    agentsCarryingDistance[i] += carryingDistance;

                    pendingTasks.erase(taskId);

                } else {

                    try {
                        std::ostringstream stream;
                        stream << "agent not found" << this->agents[i];
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }

                }

            } else {

                try {
                    std::ostringstream stream;
                    stream << "task id not found" << taskId;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }


        }

    }

    while (!agentsCarryingDistance.empty()) {

        int index = 0;
        unsigned distance = 0;

        for (int i = 0; i < agentsCarryingDistance.size(); i++) {

            if (agentsCarryingDistance[i] > distance) {
                index = i;
                distance = agentsCarryingDistance[i];
            }

        }

        auto ita = agentsTemp.begin() + index;
        auto itd = agentsCarryingDistance.begin() + index;

        agents.push_back(*ita);

        agentsCarryingDistance.erase(itd);
        agentsTemp.erase(ita);

    }

    for (int i = 0; i < agents.size(); i++) {

        //        std::map<const _ga_agent*, std::vector<const _task*>>::iterator alloc_map_it = this->allocation_map.find(agents[i]);
        auto alloc_map_it = this->allocation_map.find(agents[i]);
        if (alloc_map_it != this->allocation_map.end()) {

            unsigned jsize = alloc_map_it->second.size();

            for (int j = 0; j < jsize; j++) {

                unsigned index = i + (agents.size() * j);

                tasks[index] = alloc_map_it->second[j];

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "agent not found" << this->agents[i];
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    }

    //    std::cout << "aqui" << std::endl;

}