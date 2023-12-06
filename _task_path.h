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
#include "_stepPath.h"
#include "_stepSite.h"

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
    
    void listSites(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector, 
        unsigned offset,
        unsigned& pickup_span,
        unsigned& delivary_span,
        const std::function<bool(const PathType& type, const _site&, const _task*, const _task*)>& function) const{
        
        pickup_span = delivary_span = 0;
        
        unsigned count = 0;
        bool flag = true;
        
            listPaths(
                endpointsPathAlgorithm,
                task_vector, 
                    [&function, offset, &count, &flag, &pickup_span, &delivary_span](const PathType& type, const _path& path, const _task* current, const _task* next){
                        
//                        if(count >= offset){
                            
                            if(type == PathType::pickup){

                                pickup_span += path.size();

                            } else {

                                delivary_span += path.size();

                            }
                            
//                        }
                        
                        bool flag2 = false;
                        
                        if(flag){
                            
                            path.rlistOffset(0, [&flag2, &function, &count, offset, type, current, next](const _site& site){
                                
                                if(count >= offset){
                            
                                    flag2 = function(type, site, current, next);
                                    
                                }
                                
                                count++;
                                
                                return flag2;
                                
                            });
                            
                            flag = false;
                            
                        } else {
                            
                            path.rlistOffset(1, [&flag2, &function, &count, offset, type, current, next](const _site& site){
                            
                                if(count >= offset){
                            
                                    flag2 = function(type, site, current, next);
                                    
                                }
                                
                                count++;
                                
                                return flag2;
                            
                            });
                            
                        }
                                                
                        return flag2;
                        
                    });
        
    }
    
    void listSiteMatches(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector1,
        const std::vector<const _task*>& task_vector2,
        unsigned offset1,
        unsigned offset2,
        unsigned& pickup_span,
        unsigned& delivery_span,
        const std::function<bool(const PathType&, const PathType&, const _site&, const _site&)>& function,
        unsigned colision = 0) const{
        
        std::vector<const _site*> sv1, sv2;
        std::vector<PathType> tv1, tv2;
        
        unsigned i1, i2;
        
        listSites(
            endpointsPathAlgorithm, 
            task_vector1, 
            offset1,
            pickup_span,
            delivery_span,
            [&sv1, &tv1](const PathType& type, const _site& site, const _task* c1, const _task* c2){
                
                sv1.push_back(&site);
                tv1.push_back(type);
                
                return false;
                
            });
            
        listSites(
            endpointsPathAlgorithm, 
            task_vector2, 
            offset2,
            i1,
            i2,
            [&sv2, &tv2, &sv1](const PathType& type, const _site& site, const _task* c1, const _task* c2){
                                                
                sv2.push_back(&site);
                tv2.push_back(type);
                
                return false;
                
            });
            
//        pickup_span = std::max<unsigned>(pickup_span, i1);
//        delivery_span = std::max<unsigned>(delivery_span, i2);
            
        if(colision > 0){
            
            if(sv1.size() > sv2.size()){

                for (int i = 0; i < sv2.size(); i++) {
                    
                    bool flag = false;
                    
                    sv1[i]->listNeighbors([&flag, i, &tv1, &tv2, &sv2, &sv1, &function](const _site& neight){
                        
                        if(sv2[i]->match(neight)){
                            if(function(tv1[i], tv2[i], *sv1[i], *sv2[i])){
                                flag = true;
                                return true;
                            }
                        }
                        
                        return flag;
                        
                    });

                    if(flag) break;

                }


            } else {

                for (int i = 0; i < sv1.size(); i++) {

                    bool flag = false;
                    
                    sv1[i]->listNeighbors([&flag, i, &tv1, &tv2, &sv2, &sv1, &function](const _site& neight){
                        
                        if(sv2[i]->match(neight)){
                            if(function(tv1[i], tv2[i], *sv1[i], *sv2[i])){
                                flag = true;
                                return true;
                            }
                        }
                        
                        return flag;
                        
                    });

                    if(flag) break;

                }

            }
            
            

        } else {

            if(sv1.size() > sv2.size()){

                for (int i = 0; i < sv2.size(); i++) {

                    if(sv2[i]->match(*sv1[i]))
                        if(function(tv1[i], tv2[i], *sv1[i], *sv2[i]))
                            break;

                }


            } else {

                for (int i = 0; i < sv1.size(); i++) {

                    if(sv2[i]->match(*sv1[i]))
                        if(function(tv1[i], tv2[i], *sv1[i], *sv2[i]))
                            break;

                }

            }
            
        }      
                
    }
            
    bool checkMatchesWithStepPath(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector,
        const _stepPath& path,
        _stepSite& site,
        PathType& type,
        unsigned& pickup_span,
        unsigned& delivary_span,
        unsigned colision = 0){
        
        pickup_span = delivary_span = 0;
        
        bool ret = false;
                
        _stepPath pathAux = path;
        unsigned offset = path.currentSite().GetStep();
        
        if(colision > 0){
            
            listSites(
                endpointsPathAlgorithm, 
                task_vector, 
                offset,
                pickup_span,
                delivary_span,
                [&site, &type, &pathAux, &ret](const PathType& t, const _site& s, const _task* c1, const _task* c2){

                    if(pathAux.empty()) return true;
                    
                    s.listNeighbors([&site, &type, t, &pathAux, &ret](const _site& neight){
                        
                        if(pathAux.currentSite().match(neight)){

                            ret = true;
                            type = t;
                            site = pathAux.currentSite();

                        }
                        
                        return ret;
                        
                    });
                    
                    pathAux.pop();

                    return ret;

                });            
            
            
        } else {
                
            listSites(
                endpointsPathAlgorithm, 
                task_vector, 
                offset,
                pickup_span,
                delivary_span,
                [&site, &type, &pathAux, &ret](const PathType& t, const _site& s, const _task* c1, const _task* c2){

                    if(pathAux.empty()) return true;

                    if(pathAux.currentSite().match(s)){

                        ret = true;
                        type = t;
                        site = pathAux.currentSite();

                    }
                    
                    pathAux.pop();

                    return ret;

                });
            
        }
            
        return ret;
        
    }
    
    void countMatchesWithStepPath(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector,
        const _stepPath& path,
        unsigned& pickup,
        unsigned& delivery,
        unsigned& pickup_span,
        unsigned& delivary_span,
        unsigned colision = 0){
        
        pickup = delivery = pickup_span = delivary_span = 0;
                
        _stepPath pathAux = path;
        unsigned offset = path.currentSite().GetStep();
        
        if(colision > 0){
            
            listSites(
                endpointsPathAlgorithm, 
                task_vector, 
                offset,
                pickup_span,
                delivary_span,
                [&pickup, &delivery, &pathAux](const PathType& type, const _site& site, const _task* c1, const _task* c2){

                    if(pathAux.empty()) return true;
                                        
                    site.listNeighbors([&pickup, &delivery, &pathAux, type](const _site& neight){
                        
                        if(pathAux.currentSite().match(neight)){

                            if(type == PathType::pickup){

                                pickup++;

                            } else {

                                delivery++;

                            } 

                        }
                        
                        return false;
                        
                    });

                    pathAux.pop();

                    return false;

                });
            
        }else{
                
            listSites(
                endpointsPathAlgorithm, 
                task_vector, 
                offset,
                pickup_span,
                delivary_span,
                [&pickup, &delivery, &pathAux](const PathType& type, const _site& site, const _task* c1, const _task* c2){

                    if(pathAux.empty()) return true;

                    if(pathAux.currentSite().match(site)){

                        if(type == PathType::pickup){

                            pickup++;

                        } else {

                            delivery++;

                        }             

                    }

                    pathAux.pop();

                    return false;

                });
            
        }
        
    }
        
    bool checkMatches(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector1,
        const std::vector<const _task*>& task_vector2,
        unsigned offset1,
        unsigned offset2,
        PathType& type1,
        PathType& type2,
        unsigned& pickup_span,
        unsigned& delivary_span,
        unsigned colision = 0)const{
        
        pickup_span = delivary_span = 0;
        
        bool ret = false;
                    
        listSiteMatches(
            endpointsPathAlgorithm,
            task_vector1,
            task_vector2,
            offset1,
            offset2,
            pickup_span,
            delivary_span,
            [&ret, &type1, &type2](const PathType& t1, const PathType& t2, const _site& s1, const _site& s2){

                ret = true;
                type1 = t1;
                type2 = t2;

                return ret;

            },
            colision);
            
        
            
        return ret;
        
    }
    
    void countMatches(
        const _endpointsPathAlgorithm* endpointsPathAlgorithm,
        const std::vector<const _task*>& task_vector1,
        const std::vector<const _task*>& task_vector2,        
        unsigned offset1,
        unsigned offset2,
        unsigned& pickup1,
        unsigned& pickup2,        
        unsigned& delivery1,
        unsigned& delivery2,
        unsigned& pickup_span,
        unsigned& delivary_span,
        unsigned colision = 0) const{
                
        pickup1 = pickup2 = delivery1 = delivery2 = pickup_span = delivary_span = 0;
        
        listSiteMatches(
            endpointsPathAlgorithm,
            task_vector1,
            task_vector2,
            offset1,
            offset2,
            pickup_span,
            delivary_span,
            [&pickup1, &pickup2, &delivery1, &delivery2](const PathType& t1, const PathType& t2, const _site& s1, const _site& s2){
                
                if(t1 == PathType::pickup){
                    pickup1++;
                } else {
                    delivery1++;
                }
                
                if(t2 == PathType::pickup){
                    pickup2++;
                } else {
                    delivery2++;
                }
                                
                return false;
                
            },
            colision);
        
    }
    
    
    
    unsigned getPortion() const {
        return portion;
    }
    
private:
    
    unsigned portion = 1;
    
};

#endif /* _TASK_PATH_H */

