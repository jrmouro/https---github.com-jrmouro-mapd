/* 
 * File:   _task_path.h
 * Author: ronaldo
 *
 * Created on 23 de novembro de 2023, 15:23
 */

#ifndef _TASK_PATH_H
#define _TASK_PATH_H

#include <vector>
#include <functional>
#include "_task.h"
#include "_path.h"
#include "_endpointsPathAlgorithm.h"

class _task_path {
    
public:
    
    enum PathType{
        pickup,
        delivery
    };
    
    _task_path(unsigned portion = 1) :
    portion(portion){ }
    
    _task_path(const _task_path& other) :
    portion(other.portion){ }   
        
    void listPortionPaths(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector, 
        const std::function<bool(const PathType& type, const _path& path, const _task*, const _task*)>& function) const{
                       
        if(!task_vector.empty()){
            
            bool flag = true;
            int i = 0;
            for (; i < task_vector.size() - 1; i++) {
                
                const _path& path_delirery = endpointsPathAlgorithm->solve_path(task_vector[i]->getPickup(), task_vector[i]->getDelivery());
                const _path& path_pickup = endpointsPathAlgorithm->solve_path(task_vector[i]->getDelivery(), task_vector[i + 1]->getPickup());
                
                path_delirery.rlistPortion(portion, [&flag, &function, &task_vector, i](const _path& path_portion){
                    
                    if(function(PathType::delivery, path_portion, task_vector[i], task_vector[i + 1])){
                        
                        flag = false;
                        
                        return true; 
                        
                    }
                    
                    return false;
                    
                });
                
                if(flag){
                
                    path_pickup.rlistPortion(portion, [&flag, &function, &task_vector, i](const _path& path_portion){

                        if(function(PathType::pickup, path_portion, task_vector[i], task_vector[i + 1])){

                            flag = false;

                            return true; 

                        }

                        return false;

                    });
                
                }


            }
            
            if(flag){
                
                const _path& path_delirery = endpointsPathAlgorithm->solve_path(task_vector[i]->getPickup(), task_vector[i]->getDelivery());
                
                path_delirery.rlistPortion(portion, [&flag, &function, &task_vector, i](const _path& path_portion){
                    
                    if(function(PathType::delivery, path_portion, task_vector[i], nullptr)){
                        
                        flag = false;
                        
                        return true; 
                        
                    }
                    
                    return false;
                    
                });                
                
            }
            
        }

        
    }
    
    void listPaths(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector, 
        const std::function<bool(const PathType& type, const _path& path, const _task*, const _task*)>& function) const{
                       
        if(!task_vector.empty()){
            
            int i = 0;
            for (; i < task_vector.size() - 1; i++) {
                
                const _path& path_delirery = endpointsPathAlgorithm->solve_path(task_vector[i]->getPickup(), task_vector[i]->getDelivery());
                const _path& path_pickup = endpointsPathAlgorithm->solve_path(task_vector[i]->getDelivery(), task_vector[i + 1]->getPickup());
                
                if(function(PathType::delivery, path_delirery, task_vector[i], task_vector[i + 1])){
                        
                    return; 

                }
                
                if(function(PathType::pickup, path_pickup, task_vector[i], task_vector[i + 1])){

                    return; 

                }
                

            }            
                
            const _path& path_delirery = endpointsPathAlgorithm->solve_path(task_vector[i]->getPickup(), task_vector[i]->getDelivery());

            function(PathType::delivery, path_delirery, task_vector[i], nullptr);
                
        }

        
    }
    
    unsigned getPortion() const {
        return portion;
    }
    
private:
    
    unsigned portion = 1;
    
};

#endif /* _TASK_PATH_H */

