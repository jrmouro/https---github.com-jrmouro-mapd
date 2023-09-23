#include "_astarAlgorithm.h"
#include "_map.h"

_astarAlgorithm::_astarAlgorithm() {}

_astarAlgorithm::_astarAlgorithm(const _astarAlgorithm& other) {}

bool _astarAlgorithm::solve(const _map& map, const _site& start, const _site& goal, _path& path) const {

    bool ret = false;
    ClosedStates closedStates(map.getColumn_size(), map.getRow_size() * map.getColumn_size());
    PriorityStates priorityStates;
    std::vector<AstarState*> visitedStates;

    auto bstart = _site(start.GetRow(), start.GetColunm());

    AstarState* startState = new AstarState(.0f, this->heuristic(start, goal), bstart, nullptr);
    AstarState* solved = this->solveAux_iterative(startState, map, goal, closedStates, priorityStates, visitedStates);

    //        std::cout << map << std::endl;

    if (solved != nullptr) {

        path.clear();

        while (solved != nullptr) {
            path.add(solved->getSite());
            solved = solved->getPrevious();
        }

        //            path.pop(); // retira o site inicial

        ret = true;

    }

    for (auto elem : visitedStates) {

        delete elem;

    }



    return ret;

}

_astarAlgorithm::~_astarAlgorithm() {}

float _astarAlgorithm::heuristic(const _site& start, const _site& goal) const {

    return std::abs((int) start.GetRow() - (int) goal.GetRow()) + std::abs((int) start.GetColunm() - (int) goal.GetColunm());

}

_astarAlgorithm::AstarState* _astarAlgorithm::solveAux_recursive(
        AstarState* current,
        const _map& map,
        const _site& goal,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        std::vector<AstarState*>& visitedStates)const {

    visitedStates.push_back(current);

    if (goal.GetRow() == current->getSite().GetRow() && goal.GetColunm() == current->getSite().GetColunm()) {

        return current;

    }

    map.listNeighborFreeSites(current->getSite(), [current, goal, &closedStates, &priorityStates, this](const _site & site) {

        if (!closedStates.closed(site)) {

            auto state = new AstarState(current->getTraveled() + 1, this->heuristic(site, goal), site, current);

            priorityStates.push(state);

            closedStates.add(state);

        }

        return false;

    });

    if (!priorityStates.empty()) {

        auto state = priorityStates.pop();

        return this->solveAux_recursive(state, map, goal, closedStates, priorityStates, visitedStates);

    }

    return nullptr;

}

_astarAlgorithm::AstarState* _astarAlgorithm::solveAux_iterative(
        AstarState* current,
        const _map& map,
        const _site& goal,
        ClosedStates& closedStates,
        PriorityStates& priorityStates,
        std::vector<AstarState*>& visitedStates) const {

    while (current != nullptr) {

        visitedStates.push_back(current);

        if (goal.GetRow() == current->getSite().GetRow() && goal.GetColunm() == current->getSite().GetColunm()) {

            return current;

        }

        map.listNeighborFreeSites(current->getSite(), [current, goal, &closedStates, &priorityStates, this](const _site & site) {


            if (!closedStates.closed(site)) {

                auto state = new AstarState(current->getTraveled() + 1, this->heuristic(site, goal), site, current);

                closedStates.add(state);

                priorityStates.push(state);

            }

            return false;

        });

        if (!priorityStates.empty()) {

            current = priorityStates.pop();

        } else {

            current = nullptr;

        }

    }

    return nullptr;

}

_astarAlgorithm::AstarState::AstarState(float traveled, float heuristic, const _site& site, AstarState* previous) :
        traveled(traveled), _cost(heuristic + traveled), site(site), previous(previous) {}
        

_astarAlgorithm::AstarState::AstarState(const AstarState& other) :
traveled(other.traveled), _cost(other._cost), site(other.site), previous(other.previous) { }

_astarAlgorithm::AstarState::~AstarState(){ }

const _site _astarAlgorithm::AstarState::getSite() const {
    return site;
}

float _astarAlgorithm::AstarState::cost() const{
    return this->_cost; 
}

float _astarAlgorithm::AstarState::getTraveled() const {
    return traveled;
}

bool _astarAlgorithm::AstarState::operator<(const AstarState& right) const {
    return this->cost() < right.cost();
}

_astarAlgorithm::AstarState* _astarAlgorithm::AstarState::getPrevious() const {
    return this->previous;
}

bool _astarAlgorithm::AstarStateComparison::operator() (const AstarState* as1, const AstarState* as2) const {
    return *as2 < *as1;
}

_astarAlgorithm::PriorityStates::~PriorityStates(){
            
    while (!this->states.empty()){
        delete states.top();
       this->states.pop();
    }

}

void _astarAlgorithm::PriorityStates::push(AstarState*  state){

    this->states.push(state);

}

_astarAlgorithm::AstarState* _astarAlgorithm::PriorityStates::pop(){
    if(this->states.empty())
        return nullptr;

    auto ret = this->states.top();
    this->states.pop();
    return ret;
}

bool _astarAlgorithm::PriorityStates::empty(){
    return this->states.empty();
}

void _astarAlgorithm::PriorityStates::clear(){            
    while (!this->states.empty())
       this->states.pop();           
}


_astarAlgorithm::ClosedStates::ClosedStates(unsigned colunm, unsigned size) :
    colunm(colunm), states(size, false) {}

_astarAlgorithm::ClosedStates::ClosedStates(const ClosedStates& other) :
colunm(other.colunm), states(other.states) { }

void _astarAlgorithm::ClosedStates::add(AstarState* state){
    states[
        state->getSite().GetRow() * colunm + 
        state->getSite().GetColunm()] = true;             
}

bool _astarAlgorithm::ClosedStates::closed(const _site& site){

    return states[
        site.GetRow() * colunm + 
        site.GetColunm()];

}
