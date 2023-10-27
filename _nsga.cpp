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
//#include "_ga_solution.h"


_nsga::_nsga(
    const std::function<bool(const _ga_solution&, unsigned)>& stopCondition,
    unsigned population_size_max, 
    unsigned population_size_min, 
    unsigned mutation_children_distribution_size,
    unsigned generic_distribution_size,        
    unsigned seed) :
        population_size_max(population_size_max), 
        population_size_min(population_size_min), 
        mutation_children_distribution_size(mutation_children_distribution_size),
        generic_distribution_size(generic_distribution_size),
        stopCondition(stopCondition),
        seed(seed){ }

_nsga::_nsga(const _nsga& other) :
        population_size_max(other.population_size_max), 
        population_size_min(other.population_size_min), 
        mutation_children_distribution_size(other.mutation_children_distribution_size), 
        generic_distribution_size(other.generic_distribution_size), 
        seed(other.seed), 
        stopCondition(other.stopCondition) { }


_allocation* _nsga::solve(const _ga_token& token) const{

    _ga_solution* solution = new _ga_solution();

    solve(token, *solution);

    return solution;

}

void _nsga::solve(const _ga_token& token, _ga_solution& solution) const {
        
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<unsigned> distribution(0, generic_distribution_size);
    
    unsigned generation = 0;

    _ga_solution greedy(token); //TODO        

    _ga_solution* best = &greedy;

    _ga_population population(greedy, generator, population_size_max, population_size_min);

    while(true){
        
        std::cout << "generation: " << generation << std::endl;
        std::cout << "solution: " << *best << std::endl;

        expand_population(token, generator, population);

        best = reduce_population(token, generator, population);
        
        if(best != nullptr){

            if(stopCondition(*best, generation++)) break;
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "nill solution fail";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            } 
            
        }

    }

    solution = *best;

}

void _nsga::expand_population(const _ga_token& token, std::default_random_engine& generator, _ga_population& population) const {

    std::uniform_int_distribution<unsigned> distribution(0, generic_distribution_size);
    std::uniform_int_distribution<unsigned> mutation_children_distribution(0, mutation_children_distribution_size);

    std::map<_ga_solution*, unsigned> solution_border_map;
    std::vector<std::vector<_ga_solution*>> borders;
    nds_population(token, population, borders, solution_border_map);

    while(population.isExpandable()){

        auto parents = crossover_select_parents(token, population,  generator, solution_border_map);

        auto children = parents.first->get_crossover(*parents.second);

        population.add(children.first);

        if(mutation_children_distribution(generator) == 0){

            children.first->disturb(distribution(generator));

        }

        if(population.isExpandable()){

            population.add(children.second);

            if(mutation_children_distribution(generator) == 0){

                children.second->disturb(distribution(generator));

            }

        } else {

            delete children.second;

        }                

    }

}

_ga_solution* _nsga::reduce_population(const _ga_token& token, std::default_random_engine& generator, _ga_population& population) const {

    std::map<_ga_solution*, unsigned> solution_border_map;
    
    std::vector<std::vector<_ga_solution*>> borders;
    nds_population(token, population, borders, solution_border_map);

    auto rit = borders.rbegin();

    for(; rit != borders.rend(); ++rit){

        for (auto solution : *rit) {

            if(population.isReducible()){

                bool result = population.remove(solution);

                if(!result){

                    try {
                        std::ostringstream stream;
                        stream << "remove solution fail: " << solution;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    } 

                }

            }

        }

    }
    
    if(!borders.empty()){
        
        if(borders.at(0).size() > 2){
    
            std::map<_ga_solution*, float> solution_distance_map;

            return crowding_distance(token, borders.at(0), solution_distance_map); 
        
        } if(borders.at(0).size() > 1){
            
            std::uniform_int_distribution<unsigned> distribution(0, 1);
            
            return borders.at(0).at(distribution(generator));
            
        } if(borders.at(0).size() > 0){
            
            return borders.at(0).at(0);
            
        }
    
    }
    
    return nullptr;

}

std::pair<_ga_solution*, _ga_solution*> _nsga::crossover_select_parents(
    const _ga_token& token, 
    const _ga_population& population, 
    std::default_random_engine& generator,
    const std::map<_ga_solution*, unsigned>& solution_border_map) const {

    _ga_solution *solutionA, *solutionB;
    
    tournament_selection(token, population, solution_border_map, generator, &solutionA);
    tournament_selection(token, population, solution_border_map, generator, &solutionB);


    while(solutionA == solutionB){

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
                    stream << "solution not mapped: " << *sb;
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

        unsigned msp = solution->evaluate(token, _ga_solution::EvalType::makespan);
        unsigned eng = solution->evaluate(token, _ga_solution::EvalType::energy);

        msp_min = std::min<unsigned>(msp_min, msp);
        msp_max = std::max<unsigned>(msp_max, msp);
        eng_min = std::min<unsigned>(eng_min, eng);
        eng_max = std::max<unsigned>(eng_max, eng);

        bool flag = true;
        for(auto it = sort_border.begin(); it != sort_border.end(); it++){

            if(msp < (*it)->evaluate(token, _ga_solution::EvalType::makespan)){

                sort_border.insert(it, solution);
                flag = false;
                break;

            }              

        }

        if(flag) sort_border.push_back(solution);            

    }

    float div = (float)msp_max - (float)msp_min;

    for(auto it = sort_border.begin() + 1; it != sort_border.end() - 1; it++){

        float sa = (float)(*(it - 1))->evaluate(token, _ga_solution::EvalType::makespan);
        float sp = (float)(*(it + 1))->evaluate(token, _ga_solution::EvalType::makespan);

        float dist = std::abs(sa - sp) / div;

        std::map<_ga_solution*, float>::iterator dit = solution_distance_map.find(*it);
        
        if(dit !=  solution_distance_map.end()){
        
            dit->second += dist;
        
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

        unsigned eng = solution->evaluate(token, _ga_solution::EvalType::energy);

        bool flag = true;
        for(auto it = sort_border.begin(); it != sort_border.end(); it++){

            if(eng < (*it)->evaluate(token, _ga_solution::EvalType::energy)){

                sort_border.insert(it, solution);
                flag = false;
                break;

            }              

        }

        if(flag) sort_border.push_back(solution);            

    }


    div = (float)eng_max - (float)eng_min;

    for(auto it = sort_border.begin() + 1; it != sort_border.end() - 1; it++){

        float sa = (float)(*(it - 1))->evaluate(token, _ga_solution::EvalType::energy);
        float sp = (float)(*(it + 1))->evaluate(token, _ga_solution::EvalType::energy);

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

    population.listSolutions([border_index, &dominated_count_map, &domination_map, &nextBorder, &solution_border_map, population, token](unsigned index, _ga_solution* solution1){

        auto ins_pair = domination_map.insert(std::pair<_ga_solution*, std::vector<_ga_solution*>>(solution1, std::vector<_ga_solution*>()));

        unsigned n = 0;

        population.listSolutions([&n, solution1, token, &ins_pair](unsigned index, _ga_solution* solution2){

            if(solution1 != solution2){

                if(solution1->dominate(token, *solution2)){

                    ins_pair.first->second.push_back(solution2);

                } else if(solution2->dominate(token, *solution1)){

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

    std::vector<_ga_solution*>& currentBorder = borders.at(border_index++);

    while(!currentBorder.empty()){

        nextBorder.clear();

        for (auto solution1 : currentBorder) {

            auto dom = domination_map.find(solution1);

            if(dom != domination_map.end()){

                for (auto solution2 : dom->second) {

                    auto s2nit = dominated_count_map.find(solution2);

                    unsigned n = s2nit->second - 1;

                    s2nit->second = n;

                    if(n == 0){

                        nextBorder.push_back(solution2);    
                        solution_border_map.insert(std::pair<_ga_solution*, unsigned>(solution2, border_index));

                    }

                }

            }

        }

        borders.push_back(nextBorder);

        currentBorder = borders.at(border_index++);

    } 

}

