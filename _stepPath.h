/* 
 * File:   _stepPath.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 13:58
 */

#ifndef _STEPPATH_H
#define _STEPPATH_H

#include <climits>
#include <vector>
#include <functional>
#include "_stepSite.h"
#include "MapdException.h"
#include "_path.h"

class _stepPath{
public:
    
    _stepPath() {}
    
    _stepPath(const _stepSite& site) {
        sites.push_back(site);
    }
    
    _stepPath(const _stepPath& orig) : sites(orig.sites) {}
    
    _stepPath& operator=(const _stepPath& right) {
        if (this == &right)
            return *this;
        this->sites.clear();
        for (auto site : right.sites) {
            this->sites.push_back(site);
        }
        return *this;
    }

    
    virtual ~_stepPath() {}

    void add(const _stepSite& site) {

        this->sites.push_back(site);

    }
    
    void progress(const _stepSite& site){       
        
        std::vector<_stepSite>::iterator it = this->sites.begin();
        this->sites.insert(it, site);       
    
    }
    
    void progress(const _stepPath& path){       
        
        path.forward([this](const _stepSite& site){
            
            this->progress(site);
            
            return false;
            
        });        
    
    }
        
    const _stepSite& goalSite()const{
        
        if(this->sites.empty()){
            
             try {
                std::ostringstream stream;
                stream << "invalid base on empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }  
        
        return this->sites.front();
        
    }
    
    const _stepSite& currentSite() const{
                
        if(this->sites.empty()){
            
             try {
                std::ostringstream stream;
                stream << "invalid top on empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }  
        
        return this->sites.back();
        
    }
    
    const _stepSite& futureSite() const{
                
        if(this->sites.size() < 2){
            
             try {
                std::ostringstream stream;
                stream << "invalid future site";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }   
        
        return this->sites[sites.size() - 2];
        
    }
    
    _stepSite get(unsigned step) const {
        
        if(step <= goalSite().GetStep() && step >= currentSite().GetStep()){
            
            unsigned aux = goalSite().GetStep() - step;
            
            return sites.at(aux);
            
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
        
        return _stepSite();
        
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
        
        return _stepSite();
        
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
    
    bool empty() const {
        return this->sites.empty();
    }
        
    void backward(const std::function<bool(const _stepSite&)>& function, unsigned begin = 0) const {
        
        std::vector<_stepSite>::const_iterator it = sites.cbegin() + begin;
        
        for(; it != sites.cend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void forward(const std::function<bool(const _stepSite&)>& function, unsigned begin = 0) const {
        
        std::vector<_stepSite>::const_reverse_iterator it = sites.crbegin() + begin;
        
        for(; it != sites.crend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void movingList(const std::function<bool(const _stepSite&, const _stepSite&)>& function, unsigned begin = 0) const {
        
        std::vector<_stepSite>::const_reverse_iterator it = sites.crbegin() + begin;
        
        for(; it != sites.crend() - 1; it++){
            
            if(function(*it, *(it + 1))) return;
            
        }
        
    }
    
    static bool trivialSteppingPath(_stepPath& path){
        
        if(path.size() == 1){
            
            _stepSite trivial(path.currentSite());
            trivial.SetStep(trivial.GetStep() + 1);
            path.progress(trivial);
            return true;
        } 
        
        try {
            std::ostringstream stream;
            stream << "invalid empty path" << std::endl;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
        return false;
        
    }
    
    bool isTrivial()const{
        return sites.size() == 1;
    }    
    
    unsigned stepToArrive(const _site& goal) const {
        
        unsigned count = 0;
            
        std::vector<_stepSite>::const_reverse_iterator it = sites.crbegin();
        
        for(; it != sites.crend(); it++){
            
            if(goal.match(*it)){
                    
                return count;                   

            }

            count++;
            
        }       
        
        return UINT_MAX;
        
    }
    
    static void buildFromPath(unsigned initialStep, const _path& path, _stepPath& stepPath){
        
        stepPath.clear();
        
        path.rlist([&initialStep, &stepPath](const _site& site){
            
            stepPath.progress(_stepSite(initialStep++, site));            
            
            return false;
            
        });
        
    }
    
    bool collideWith(const _stepPath& other) const{
        
        if(!sites.empty() && !other.sites.empty()){
            
            unsigned initialStepThis = sites.back().GetStep();
            unsigned initialStepOther = other.sites.back().GetStep();
            int diff = initialStepOther - initialStepThis;
                    
            int i, j;
            
            if(diff < 0){
                
                i = sites.size() - 1;
                j = other.sites.size() - 1 + diff;
                
            } else {
                
                i = sites.size() - 1 - diff;
                j = other.sites.size() - 1;
                                                
            }
            
            for (; i >= 0 && j  >= 0; i--, j--) {
                    
                if(sites[i].match(other.sites[j])) return true;         

            }       
                    
        }
        
        return false;
        
    }
    
    unsigned collideCount(const _stepPath& other) const{
        
        unsigned ret = 0;
        
        if(!sites.empty() && !other.sites.empty()){
            
            unsigned initialStepThis = sites.back().GetStep();
            unsigned initialStepOther = other.sites.back().GetStep();
            int diff = initialStepOther - initialStepThis;
                    
            int i, j;
            
            if(diff < 0){
                
                i = sites.size() - 1;
                j = other.sites.size() - 1 + diff;
                
            } else {
                
                i = sites.size() - 1 - diff;
                j = other.sites.size() - 1;
                                                
            }
            
            for (; i >= 0 && j  >= 0; i--, j--) {
                    
                if(sites[i].match(other.sites[j])) ret++;         

            }       
                    
        }
        
        return ret;
        
    }
    
    
        
private:
        
    std::vector<_stepSite> sites;
    
};

#endif /* _STEPPATH_H */

