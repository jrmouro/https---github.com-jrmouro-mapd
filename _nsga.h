/* 
 * File:   _nsga.h
 * Author: ronaldo
 *
 * Created on 22 de outubro de 2023, 08:31
 */

#ifndef _NSGA_H
#define _NSGA_H

#include "_ga_population.h"
#include <cmath>
#include <climits>


class _nsga {
public:
    virtual void solve(const _ga_token& token, _ga_solution& solution) const {
        
        unsigned generation = 0;
        
        greedy(token, solution);
        
        _ga_population population(solution, seed, population_size_max, population_size_min);
                        
        while(true){
            
            expand_population(token, population);
            reduce_population(token, population);
            update_solution(token, population, solution);
            
            if(!stopCondition(solution, generation)) break;
            
        }
        
    }
    
protected:
    
    virtual void greedy(const _ga_token& token, _ga_solution& solution) const{ //TODO Melhorar
        
        solution.evaluate(token); 
        
    }
    
    virtual void tournament_selection(
        const _ga_token& token, 
        const _ga_population& population, 
        const std::map<_ga_solution*, unsigned>& solution_border_map,
        const unsigned seed,
        _ga_solution** solution){
        
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<unsigned> distribution(0, 1000000);
        
        _ga_solution* sa = population.get_random(distribution(generator));
        _ga_solution* sb = population.get_random(distribution(generator));
        
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
    
    virtual void crowding_distance(
            const _ga_token& token,
            const std::vector<_ga_solution*>& border, 
            std::map<_ga_solution*, float>& solution_distance_map) const{
        
        distances.clear();
        *max_distance = nullptr;
        
        unsigned msp_min = UINT_MAX, msp_max = 0, eng_min = UINT_MAX, eng_max = 0;
        std::vector<_ga_solution*>& sort_border;
        
        
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
            
            float dist = std::abs<float>(sa - sp) / div;
            
            std::map<_ga_solution*, float>::iterator dit = solution_distance_map.find(*it);
            dit->second += dist;

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
        
        
        float div = (float)eng_max - (float)eng_min;
        
        for(auto it = sort_border.begin() + 1; it != sort_border.end() - 1; it++){
                
            float sa = (float)(*(it - 1))->evaluate(token, _ga_solution::EvalType::energy);
            float sp = (float)(*(it + 1))->evaluate(token, _ga_solution::EvalType::energy);
            
            float dist = std::abs<float>(sa - sp) / div;
            
            std::map<_ga_solution*, float>::iterator dit = solution_distance_map.find(*it);
            dit->second += dist;

        }
                
    }
    
    virtual void nds_population(const _ga_token& token, const _ga_population& population, std::map<_ga_solution*, unsigned>& solution_border_map) const {
                
        std::vector<std::vector<_ga_solution*>> borders;
        std::map<_ga_solution*, std::vector<_ga_solution*>> domination_map;
        std::map<_ga_solution*, unsigned> dominated_count_map;
        std::vector<_ga_solution*> nextBorder;
                
        unsigned border_index = 0;  
                
        population.listConstSolutions([&dominated_count_map, &domination_map, &nextBorder](unsigned index, const _ga_solution& solution1){
            
            auto ins_pair = domination_map.insert(std::pair<_ga_solution*, std::vector<_ga_solution*>>(&solution1, std::vector<_ga_solution*>()));
            
            unsigned n = 0;
            
            population.listConstSolutions([&n, &domination_map, solution1, &borders](unsigned index, const _ga_solution& solution2){
            
                if(&solution1 != &solution2){
                    
                    if(solution1.dominate(token, solution2)){
                        
                        ins_pair->first.push_back(&solution2);
                        
                    }else if(solution2.dominate(token, solution1)){
                        
                        n = n + 1;
                        
                    }
                    
                }

                return false;

            }) ;
            
            if(n == 0){
                
                nextBorder.push_back(&solution1);
                
            }
            
            dominated_count_map.insert(std::pair<_ga_solution*, unsigned>(&solution1, n));           
                        
            return false;
            
        }) ; 
        
        borders.push_back(nextBorder);
        
        std::vector<_ga_solution*>& currentBorder = borders.at(border_index++);
                 
        while(!currentBorder.empty()){
            
            nextBorder.clear();
        
            for (auto solution1 : border_it->second) {

                auto dom = domination_map.find(solution1);

                if(dom != domination_map.end()){

                    for (auto solution2 : dom->second) {
                        
                        auto s2nit = dominated_count_map.find(solution2);
                        
                        unsigned n = s2nit->second - 1;
                        
                        s2nit->second = n;

                        if(n == 0){
                            
                            nextBorder.push_back(solution2);                            
                            
                        }

                    }

                }

            }
            
            borders.push_back(nextBorder);
            
            currentBorder = borders.at(border_index++);
       
        }   
        
        solution_border_map.clear();
        
        border_index = 0;
        
        for (auto border : borders) {
            
            for (auto solution : border) {
                
                solution_border_map.insert(std::pair<_ga_solution*, unsigned>(solution, border_index));

            }
            
            border_index++;
            
        }

                
    }
    
    
    
    virtual void expand_population(const _ga_token& token, _ga_population& population) const {
                
        
        
    }
    
    virtual void reduce_population(const _ga_token& token, _ga_population& population) const {
                
        
        
    }
    
    virtual void update_solution(const _ga_token& token, const _ga_population& population, _ga_solution& solution) const {
                
        
        
    }
    
    const unsigned population_size_max;
    const unsigned population_size_min;
    const unsigned seed;

    const std::function<bool(const _ga_solution&, unsigned)> stopCondition;
    
};

#endif /* _NSGA_H */

