/* 
 * File:   _nsga.cpp
 * Author: ronaldo
 * 
 * Created on 22 de outubro de 2023, 08:31
 */

#include "_nsga.h"
#include <cmath>
#include "MapdException.h"
#include "_ga_population.h"
#include "_ga_objective_function.h"
//#include "_ga_solution.h"


_nsga::_nsga(
    const std::string& id,
    const _ga_objective_function& ga_objective_function, 
    const std::function<bool(const _ga_solution&, unsigned, const std::chrono::duration<double>&)>& stopCondition,
    unsigned population_size_max, 
    unsigned population_size_min, 
    unsigned solution_validity,
    float population_mutation_rate,
    float agents_crossover_point_rate,
    float tasks_crossover_point_rate,
    float agents_mutation_rate,
    float tasks_mutation_rate,       
    unsigned seed) :
        _ga_solutionAllocator(id, solution_validity),
        ga_objective_function(ga_objective_function),
        population_size_max(population_size_max), 
        population_size_min(population_size_min), 
        population_mutation_rate(population_mutation_rate),
        agents_crossover_point_rate(agents_crossover_point_rate),
        tasks_crossover_point_rate(tasks_crossover_point_rate),
        agents_mutation_rate(agents_mutation_rate),
        tasks_mutation_rate(tasks_mutation_rate),
        stopCondition(stopCondition),
        seed(seed){ }

_nsga::_nsga(const _nsga& other) :
        _ga_solutionAllocator(other),
        ga_objective_function(other.ga_objective_function),
        population_size_max(other.population_size_max), 
        population_size_min(other.population_size_min), 
        population_mutation_rate(other.population_mutation_rate),
        agents_crossover_point_rate(other.agents_crossover_point_rate),
        tasks_crossover_point_rate(other.tasks_crossover_point_rate),
        agents_mutation_rate(other.agents_mutation_rate),
        tasks_mutation_rate(other.tasks_mutation_rate),
        seed(other.seed), 
        stopCondition(other.stopCondition) { }

_nsga::~_nsga(){}

_allocation* _nsga::borrow(const _ga_token& token) {

    _ga_solution* solution = new _ga_solution(token, solution_validity); // TODO greedy?

    solve(token, *solution);
    
    borrowed.insert(solution);

    return solution;

}

_agentsTasksAllocator* _nsga::emptyClone() const {
    return new _nsga(*this);
}

_allocation* _nsga::restore(const _ga_token& token, _allocation* allocation){
    
    ((_ga_solution*)allocation)->restore(token);
    
    solve(token, *((_ga_solution*)allocation));
    
    return allocation;
    
}

void _nsga::solve(const _ga_token& token, _ga_solution& solution) const {
        
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<unsigned> distribution(0, generic_distribution_size);
    
    unsigned generation = 0;

    unsigned pop_min = std::max<unsigned>(2 * token.numberOfAgents(), population_size_min);
//    unsigned pop_max = std::min<unsigned>(pop_min * 3, population_size_max); ;
    unsigned pop_max = pop_min * 3;
    

    _ga_solution* best = &solution;
    
    _ga_population population(*best, generator, pop_max, pop_min);
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    
    while(!stopCondition(*best, generation++, time_span)){
        
//        std::cout << "generation: " << generation << std::endl;
//        std::cout << "solution: " << *best << std::endl;

        expand_population(token, *best, generator, population);
        
//        std::cout << "generation: " << generation << std::endl;
//        std::cout << "Polulation: " << std::endl << population << std::endl;        
//        std::cout << "Best solution: " << *best << std::endl;
//        std::cout << std::endl;

        best = reduce_population(token, generator, population);
        
        
//        std::cout << "generation: " << generation << std::endl;
//        std::cout << "Polulation: " << std::endl << population << std::endl;        
//        std::cout << "Best solution: " << *best << std::endl;
//        std::cout << std::endl;
        
        if(best == nullptr){
            
            try {
                std::ostringstream stream;
                stream << "nill solution fail";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            } 
            
        }
        
//        t1 = t2;
        t2 = std::chrono::high_resolution_clock::now();
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        

    }
    
//    std::cout << "solution: " << *best << std::endl;

    solution = *best;

}

void _nsga::expand_population(
        const _ga_token& token, 
        const _ga_solution& solution,
        std::default_random_engine& generator, 
        _ga_population& population) const {
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::uniform_int_distribution<unsigned> distribution(0, generic_distribution_size);
//    std::uniform_int_distribution<unsigned> mutation_children_distribution(0, mutation_children_distribution_size);

    std::map<_ga_solution*, unsigned> solution_border_map;
    std::vector<std::vector<_ga_solution*>> borders;
    nds_population(token, population, borders, solution_border_map);
    
    unsigned n_children = population.getSize_max() - population.getSize_min();
    unsigned agentsCrossoverPoint = solution.agentsSize() * agents_crossover_point_rate;        
    unsigned tasksCrossoverPoint = solution.tasksSize() * tasks_crossover_point_rate;
    unsigned pop_mutation_size = std::min<unsigned>(1, population.getSize_min() * population_mutation_rate);
    unsigned agents_mutation_size = std::min<unsigned>(1, solution.agentsSize() * agents_mutation_rate);
    unsigned tasks_mutation_size = std::min<unsigned>(1, solution.tasksSize() * tasks_mutation_rate);
    
    std::vector<_ga_solution*> children_pool(n_children, nullptr);
    
    while(n_children > 0){
        
        auto parents = crossover_select_parents(token, population,  generator, solution_border_map);       

        auto children = parents.first->get_crossover(*parents.second, agentsCrossoverPoint, tasksCrossoverPoint);
        
        children_pool[n_children - 1] = children.first.first;
        n_children--;
        
        if(n_children > 0){
            children_pool[n_children - 1] = children.second.first;
            n_children--;
        } else { break; }
        
        if(n_children > 0){
            children_pool[n_children - 1] = children.first.second;
            n_children--;
        } else { break; }
        
        if(n_children > 0){
            children_pool[n_children - 1] = children.second.second;
            n_children--;
        } else { break; }
        
    }
    
    for(int i = 0; i < pop_mutation_size; i++){
                   
        if(!children_pool.empty()){
            
            std::uniform_int_distribution<unsigned> distribution(0, children_pool.size() - 1);
            
            unsigned index =  distribution(generator);
            
            std::vector<_ga_solution*>::iterator it = children_pool.begin() + index;
            
            (*it)->disturb(agents_mutation_size, tasks_mutation_size, generator);
            
            if(!population.add(*it)){
            
                try {
                    std::ostringstream stream;
                    stream << "add solution fail: " << *it;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }
            
            children_pool.erase(it);
            
        }
        
    }
    
    for (auto child : children_pool) {
        
        if(!population.add(child)){
            
            try {
                std::ostringstream stream;
                stream << "add solution fail: " << child;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }

    }
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

//    std::cout << "expand_population time span: " << time_span.count() << std::endl;

}

_ga_solution* _nsga::reduce_population(const _ga_token& token, std::default_random_engine& generator, _ga_population& population) const {

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    std::map<_ga_solution*, unsigned> solution_border_map;
    
    std::vector<std::vector<_ga_solution*>> borders;
    nds_population(token, population, borders, solution_border_map);

    auto rit = borders.rbegin();
    bool scape = false;

    for(; rit != borders.rend() && !scape; ++rit){
        
        for(auto sit = rit->begin(); sit != rit->end(); /*sit++*/){
            
            if(population.isReducible()){

                bool result = population.remove(*sit);

                if(result){

                    sit = rit->erase(sit);

                } else {
                    
                    try {
                        std::ostringstream stream;
                        stream << "remove solution fail: " << *sit;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    } 
                    
                }

            } else {
                
                scape = true;
                break;
                
            }
            
        }

    }
    
    _ga_solution* ret = nullptr;
    
    if(!borders.empty()){
        
        if(borders.at(0).size() > 2){
                
            std::map<_ga_solution*, float> solution_distance_map;
            
            ret = crowding_distance(token, borders.at(0), solution_distance_map); 
        
        } if(borders.at(0).size() > 1){
            
            std::uniform_int_distribution<unsigned> distribution(0, 1);
            
            unsigned index = distribution(generator);
            
            ret = borders.at(0).at(index);
            
        } if(borders.at(0).size() > 0){
            
            ret = borders.at(0).at(0);
            
        }
    
    } else {
        
        try {
            std::ostringstream stream;
            stream << "empty border";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        } 
        
    }
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

//    std::cout << "reduce_population time span: " << time_span.count() << std::endl;
    
    return ret;

}

std::pair<_ga_solution*, _ga_solution*> _nsga::crossover_select_parents(
    const _ga_token& token, 
    const _ga_population& population, 
    std::default_random_engine& generator,
    const std::map<_ga_solution*, unsigned>& solution_border_map) const {

    _ga_solution *solutionA, *solutionB;
    
    tournament_selection(token, population, solution_border_map, generator, &solutionA);
    tournament_selection(token, population, solution_border_map, generator, &solutionB);


    unsigned count = 3;
    while(solutionA == solutionB && count-- > 0){

        tournament_selection(token, population, solution_border_map, generator, &solutionB);     

    }

    return std::pair<_ga_solution*, _ga_solution*>(solutionA, solutionB);

}

void _nsga::tournament_selection(
    const _ga_token& token, 
    const _ga_population& population,    
    const std::map<_ga_solution*, unsigned>& solution_border_map,
    std::default_random_engine& generator,
    _ga_solution** solution) const {

    std::uniform_int_distribution<unsigned> distribution(0, generic_distribution_size);

    _ga_solution* sa = population.get_random(generator);
    _ga_solution* sb = population.get_random(generator);

    if(sa == sb){

        *solution = sa;

    } else {

        auto ait = solution_border_map.find(sa);            

        if(ait != solution_border_map.end()){

            auto bit = solution_border_map.find(sb);

            if(bit != solution_border_map.end()){

                unsigned ba = ait->second, bb = bit->second;

                if(ba > bb){

                    *solution = sa;

                }else{

                    *solution = sb;

                }                    

            } else {

                try {
                    std::ostringstream stream;
                    stream << "solution not mapped: " << sb;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }                     

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "solution not mapped: " << *sa;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }                

        }

    }

}
_ga_solution* _nsga::crowding_distance(
        const _ga_token& token,
        const std::vector<_ga_solution*>& border, 
        std::map<_ga_solution*, float>& solution_distance_map) const{
    
    _ga_solution* ret = nullptr; 
    float crowding_distance_max = .0f;

    solution_distance_map.clear();

    unsigned msp_min = UINT_MAX, msp_max = 0, eng_min = UINT_MAX, eng_max = 0;
    std::vector<_ga_solution*> sort_border;


    for (auto solution : border) {

        solution_distance_map.insert(std::pair<_ga_solution*, unsigned>(solution, .0f));
        
        unsigned msp = ga_objective_function.eval(token, *solution, _ga_solution::EvalType::makespan);
        unsigned eng = ga_objective_function.eval(token, *solution, _ga_solution::EvalType::energy);

//        unsigned msp = solution->evaluate(token, _ga_solution::EvalType::makespan);
//        unsigned eng = solution->evaluate(token, _ga_solution::EvalType::energy);

        msp_min = std::min<unsigned>(msp_min, msp);
        msp_max = std::max<unsigned>(msp_max, msp);
        eng_min = std::min<unsigned>(eng_min, eng);
        eng_max = std::max<unsigned>(eng_max, eng);

        bool flag = true;
        for(auto it = sort_border.begin(); it != sort_border.end(); it++){
            
            unsigned msp_aux = ga_objective_function.eval(token, *(*it), _ga_solution::EvalType::makespan);

            if(msp < msp_aux){

                sort_border.insert(it, solution);
                flag = false;
                break;

            }              

        }

        if(flag) sort_border.push_back(solution);            

    }

    float div = (float)msp_max - (float)msp_min;

    for(auto it = sort_border.begin() + 1; it != sort_border.end() - 1; it++){
            
        float sa = (float)ga_objective_function.eval(token, *(*(it - 1)), _ga_solution::EvalType::makespan);
        float sp = (float)ga_objective_function.eval(token, *(*(it + 1)), _ga_solution::EvalType::makespan);
        
        float dist = std::abs(sa - sp) / div;

        std::map<_ga_solution*, float>::iterator dit = solution_distance_map.find(*it);
        
        if(dit !=  solution_distance_map.end()){
        
            dit->second += dist;
            
            if(dit->second > crowding_distance_max){
                
                crowding_distance_max = dit->second;
                
                ret = dit->first;
                
            }
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "solution not mapped: " << *(*it);
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            } 
            
        }

    }


    sort_border.clear();
    for (auto solution : border) {
        
        unsigned eng = ga_objective_function.eval(token, *solution, _ga_solution::EvalType::energy);

        bool flag = true;
        for(auto it = sort_border.begin(); it != sort_border.end(); it++){
            
            unsigned eng_aux = ga_objective_function.eval(token, *(*it), _ga_solution::EvalType::energy);

            if(eng < eng_aux){

                sort_border.insert(it, solution);
                flag = false;
                break;

            }              

        }

        if(flag) sort_border.push_back(solution);            

    }


    div = (float)eng_max - (float)eng_min;

    for(auto it = sort_border.begin() + 1; it != sort_border.end() - 1; it++){
        
        float sa = (float)ga_objective_function.eval(token, *(*(it - 1)), _ga_solution::EvalType::energy);
        float sp = (float)ga_objective_function.eval(token, *(*(it + 1)), _ga_solution::EvalType::energy);

        float dist = std::abs(sa - sp) / div;

        std::map<_ga_solution*, float>::iterator dit = solution_distance_map.find(*it);
        
        if(dit !=  solution_distance_map.end()){
        
            dit->second += dist;           
            
            if(dit->second > crowding_distance_max){
                
                crowding_distance_max = dit->second;
                
                ret = dit->first;
                
            }
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "solution not mapped: " << *(*it);
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            } 
            
        }

    }
    
    return ret;

}

void _nsga::nds_population(
        const _ga_token& token, 
        const _ga_population& population, 
        std::vector<std::vector<_ga_solution*>>& borders,
        std::map<_ga_solution*, unsigned>& solution_border_map) const {

    std::map<_ga_solution*, std::vector<_ga_solution*>> domination_map;
    std::map<_ga_solution*, unsigned> dominated_count_map;
    std::vector<_ga_solution*> nextBorder;

    borders.clear();
    solution_border_map.clear();

    unsigned border_index = 0;  

    population.listSolutions([border_index, &dominated_count_map, &domination_map, &nextBorder, &solution_border_map, &population, &token, this](unsigned index, _ga_solution* solution1){

        auto ins_pair = domination_map.insert(std::pair<_ga_solution*, std::vector<_ga_solution*>>(solution1, std::vector<_ga_solution*>()));

        unsigned n = 0;

        population.listSolutions([&n, solution1, &token, &ins_pair, this](unsigned index, _ga_solution* solution2){

            if(solution1 != solution2){
                
                ga_objective_function.evals(token, *solution1);
                ga_objective_function.evals(token, *solution2);

                if(solution1->isDominatedBy(token, *solution2)){

                    ins_pair.first->second.push_back(solution2);

                } else if(solution2->isDominatedBy(token, *solution1)){

                    n = n + 1;

                }

            }

            return false;

        }) ;

        if(n == 0){

            nextBorder.push_back(solution1);
            solution_border_map.insert(std::pair<_ga_solution*, unsigned>(solution1, border_index));

        }

        dominated_count_map.insert(std::pair<_ga_solution*, unsigned>(solution1, n));           

        return false;

    }) ; 

    borders.push_back(nextBorder);

    std::vector<_ga_solution*>* currentBorder = &borders.at(border_index++);

    while(!currentBorder->empty()){

        nextBorder.clear();

        for (auto solution1 : *currentBorder) {

            auto dom = domination_map.find(solution1);

            if(dom != domination_map.end()){

                for (auto solution2 : dom->second) {

                    auto s2nit = dominated_count_map.find(solution2);
                    
                    if(s2nit != dominated_count_map.end()){

                        unsigned n = s2nit->second - 1;

                        s2nit->second = n;

                        if(n == 0){

                            nextBorder.push_back(solution2);    
                            solution_border_map.insert(std::pair<_ga_solution*, unsigned>(solution2, border_index));

                        }
                    
                    } else {
                        
                        try {
                            std::ostringstream stream;
                            stream << "solution not mapped: " << *solution2;
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        } 
                        
                    }

                }

            } else {                
                
                try {
                    std::ostringstream stream;
                    stream << "solution not mapped: " << *solution1;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                } 
                
            }

        }

        borders.push_back(nextBorder);

        currentBorder = &borders.at(border_index++);       
        

    } 

}

