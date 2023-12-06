/* 
 * File:   _ga_system.cpp
 * Author: ronaldo
 * 
 * Created on 9 de outubro de 2023, 13:44
 */

#include <chrono>

#include "_ga_system.h"
#include "_ga_token.h"
#include "_stepMap.h"
#include "_taskMap.h"
#include "_agentsTasksAllocator.h"
#include "_agentsUpdatePath.h"
#include "_allocation.h"
#include "_ga_solution.h"

_ga_system::_ga_system(_agentsTasksAllocator& agentsTasksAllocator) :
                agentsTasksAllocator(agentsTasksAllocator){}
                
_ga_system::_ga_system(const _ga_system& other) :  
            agentsTasksAllocator(other.agentsTasksAllocator){
    if(other.allocation != nullptr)
        allocation = other.agentsTasksAllocator.borrowClone(other.allocation);
}

_ga_system::~_ga_system(){
                
    if(allocation != nullptr) agentsTasksAllocator.giveBack(allocation);
    
}

bool _ga_system::step(const _taskMap& taskMap, _ga_token& token){ 
        
    if(token.getCurrentStep() < token.getStepMap().getStep_size()/* && !(taskMap.getLastTask() < token.getCurrentStep() && token.isIdle())*/){
        
//        token.error_site_collision_check(); // TODO retirar
//        token.error_edge_collision_check();
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getCurrentStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        }); 
        
//        if(token.getCurrentStep() > 69)
//            token.getStepMap().stepView(token.getCurrentStep());
                
        if(allocation == nullptr){
            
            allocation = agentsTasksAllocator.borrow(token);
            allocation->revalidate();
                        
//            std::cout << "first allocation" << std::endl;
//            std::cout << "current step: " << token.getCurrentStep() << std::endl;
//            std::cout << "solution: " << *((_ga_solution*)allocation) << std::endl;

        } else if(addNewTask) {
            
            agentsTasksAllocator.giveBack(allocation);              
            allocation = agentsTasksAllocator.borrow(token);
            
//            std::cout << "new task allocation" << std::endl;
//            std::cout << "current step: " << token.getCurrentStep() << std::endl;
//            std::cout << "solution: " << *((_ga_solution*)allocation) << std::endl;

        } else if(!allocation->isValid() && token.isLate()){
            
            agentsTasksAllocator.giveBack(allocation);              
            allocation = agentsTasksAllocator.borrow(token);
            allocation->revalidate();
            
//            std::cout << "revalid allocation" << std::endl;
//            std::cout << "current step: " << token.getCurrentStep() << std::endl;
//            std::cout << "solution: "  << std::endl << *((_ga_solution*)allocation) << std::endl;
            
        }
        
        allocation->nextPlanningUpdate(token, [&token](const _ga_agent* agent, const _task* task){
            
            if(task != nullptr){
            
                return token.updateAgentTaskPath(agent->id(), task->id());
            
            } else {
                
                return token.updateAgentTaskPath(agent->id(), INT_MIN);
                
            }
            
        });
        
        if(taskMap.getLastTask() < token.getCurrentStep() && token.isIdle()){
            return false;
        }
        
        token.stepping();       
        
        return true;
        
    }
          
    return false;

}



void _ga_system::run(const _taskMap& taskMap, _ga_token& token){    
    
//    int count = 0, vcount = 0;
        
    while(token.getCurrentStep() < token.getStepMap().getStep_size()/* && !(taskMap.getLastTask() < token.getCurrentStep() && token.isIdle())*/){
        
//        token.error_site_collision_check();//TODO retirar
//        token.error_edge_collision_check();
        
        bool addNewTask = false;
        
        taskMap.listTasksByStep(token.getCurrentStep(), [&token, &addNewTask](const _task& task){

            token.addPendingTask(task);
            
            addNewTask = true;

            return false;

        });
        
//        if(token.getCurrentStep() > 65)
//            token.getStepMap().stepView(token.getCurrentStep());
        
        if(allocation == nullptr){
            
//            count++;
//            
//            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
//
//            std::cout << count << " - " << token.getCurrentStep() << " - new task allocation count.... ";

            allocation = agentsTasksAllocator.borrow(token);
            allocation->revalidate();
            
//            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();            
//            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
//            
//            std::cout << time_span.count() << std::endl;
            
            
            
//            std::cout << "first allocation" << std::endl;
//            std::cout << "current step: " << token.getCurrentStep() << std::endl;
//            std::cout << "solution: "  << std::endl << *((_ga_solution*)allocation) << std::endl;

        } else if(addNewTask) {
            
//            count++;
//            
//            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
//
//            std::cout << count << " - " << token.getCurrentStep() << " - new task allocation count.... ";

            
            agentsTasksAllocator.giveBack(allocation);              
            allocation = agentsTasksAllocator.borrow(token);
            allocation->revalidate();
            
//            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();            
//            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
//            
//            std::cout << time_span.count() << std::endl;
            
//            std::cout << "new task allocation" << std::endl;
//            std::cout << "current step: " << token.getCurrentStep() << std::endl;
//            std::cout << "solution: "  << std::endl << *((_ga_solution*)allocation) << std::endl;

        } else if(!allocation->isValid()){
            
//            vcount++;
//
//            std::cout << vcount << " - validity allocation count.... ";
//
//            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            
            agentsTasksAllocator.giveBack(allocation);              
            allocation = agentsTasksAllocator.borrow(token);
            allocation->revalidate();
            
//            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();            
//            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
//            
//            std::cout << time_span.count() << std::endl;
            
//            std::cout << "revalid allocation" << std::endl;
//            std::cout << "current step: " << token.getCurrentStep() << std::endl;
//            std::cout << "solution: "  << std::endl << *((_ga_solution*)allocation) << std::endl;
            
        }
        
        allocation->nextPlanningUpdate(token, [&token](const _ga_agent* agent, const _task* task){
            
            if(task != nullptr){
            
                return token.updateAgentTaskPath(agent->id(), task->id());
            
            } else {
                
                return token.updateAgentTaskPath(agent->id(), INT_MIN);
                
            }
            
        });
        
        if(taskMap.getLastTask() < token.getCurrentStep() && token.isIdle()){
            break;
        }
        
        token.stepping();
        
    }
    
    
//    std::cout << "sistem allocation" << std::endl;
//    std::cout << "current step: " << token.getCurrentStep() << std::endl;
//    std::cout << "solution: "  << std::endl << *((_ga_solution*)allocation) << std::endl;
    
}