/* 
 * File:   _stepPath.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 13:58
 */

#ifndef _STEPPATH_H
#define _STEPPATH_H

#include <vector>
#include <functional>
#include "_stepSite.h"
#include "MapdException.h"

class _stepPath{
public:
    
    _stepPath() {}
    
    _stepPath(const _stepSite& site) {
        sites.push_back(site);
    }
    
    _stepPath(const _stepPath& orig) : sites(orig.sites) {}
    
    virtual ~_stepPath() {}

    void add(const _stepSite& site) {

        this->sites.push_back(site);

    }
    
    void progress(const _stepSite& site){       
        
        std::vector<_stepSite>::iterator it = this->sites.begin();
        this->sites.insert(it, site);       
    
    }
    
    void progress(const _stepPath& path){       
        
        path.rlist([this](const _stepSite& site){
            
            this->progress(site);
            
            return false;
            
        });        
    
    }
        
    const _stepSite& goalSite()const{
        
        if(this->sites.size() > 0){   
            
            return this->sites.front();
            
        } else {
            
             try {
                std::ostringstream stream;
                stream << "invalid base on empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }     
    }
    
    const _stepSite& currentSite() const{
        
        if(this->sites.size() > 0){   
            
            return this->sites.back();
            
        } else {
            
             try {
                std::ostringstream stream;
                stream << "invalid top on empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }     
    }
    
    _stepSite pop(){
        
        if(this->sites.size() > 0){     
            _stepSite site = this->sites.back();
            this->sites.pop_back();
            return site;
        } else {
            
             try {
                std::ostringstream stream;
                stream << "invalid pop on empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }     
    }
    
    unsigned size() const{
        
        return this->sites.size();
        
    }

    friend std::ostream& operator<<(std::ostream& os, const _stepPath& obj) {
        for (std::vector<_stepSite>::const_reverse_iterator it = obj.sites.rbegin(); it != obj.sites.rend(); ++it)
            os << *it << std::endl;
        return os;
    }

    void clear(){
        this->sites.clear();
    }
    
    bool empty(){
        return this->sites.empty();
    }
        
    void list(const std::function<bool(const _stepSite&)>& function) const {
        
        for(std::vector<_stepSite>::const_iterator it = sites.cbegin(); it != sites.cend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void rlist(const std::function<bool(const _stepSite&)>& function) const {
        
        for(std::vector<_stepSite>::const_reverse_iterator it = sites.crbegin(); it != sites.crend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void moveList(const std::function<bool(const _stepSite&, const _stepSite&)>& function) const {
        
        for(std::vector<_stepSite>::const_reverse_iterator it = sites.crbegin(); it != sites.crend() - 1; it++){
            
            if(function(*it, *(it + 1))) return;
            
        }
        
    }
          
        
private:
        
    std::vector<_stepSite> sites;
    
};

#endif /* _STEPPATH_H */
