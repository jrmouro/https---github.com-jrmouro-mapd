/* 
 * File:   _path.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 13:58
 */

#ifndef _PATH_H
#define _PATH_H

#include <vector>
#include <functional>
#include "_site.h"
#include "MapdException.h"

class _path{
public:
    
    _path() {}
    
    _path(const _site& site) {
        sites.push_back(site);
    }
    
    _path(const _path& orig) : sites(orig.sites) {}
    
    virtual ~_path() {}
    
    _path& operator=(const _path& right) {
        
        if (this == &right)
            return *this; 
        
        sites = right.sites;
        
        return *this;
    }
    
    void getReverse(_path& target)const{
        
        target.clear();
        
        std::vector<_site>::const_reverse_iterator sit = sites.crbegin();
        
        for(; sit != sites.crend(); ++sit){
            
            target.sites.push_back(*sit);
            
        }
        
    }

    void add(const _site& site) {

        this->sites.push_back(site);

    }
    
    void progress(const _site& site){       
        
        std::vector<_site>::iterator it = this->sites.begin();
        this->sites.insert(it, site);       
    
    }
    
    void progress(const _path& path){       
        
        path.rlist([this](const _site& site){
            
            this->progress(site);
            
            return false;
            
        });        
    
    }
        
    const _site& goalSite()const{
        
        if(!this->sites.empty()){   
            
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
    
    const _site& currentSite() const{
        
        if(!this->sites.empty()){   
            
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
    
    _site pop(){
        
        if(this->sites.size() > 0){     
            _site site = this->sites.back();
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

    friend std::ostream& operator<<(std::ostream& os, const _path& obj) {
        for (std::vector<_site>::const_reverse_iterator it = obj.sites.rbegin(); it != obj.sites.rend(); ++it)
            os << *it << std::endl;
        return os;
    }

    void clear(){
        this->sites.clear();
    }
    
    bool empty(){
        return this->sites.empty();
    }
        
    void list(const std::function<bool(const _site&)>& function) const {
        
        for(std::vector<_site>::const_iterator it = sites.cbegin(); it != sites.cend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void rlist(const std::function<bool(const _site&)>& function) const {
        
        for(std::vector<_site>::const_reverse_iterator it = sites.crbegin(); it != sites.crend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void moveList(const std::function<bool(const _site&, const _site&)>& function) const {
        
        for(std::vector<_site>::const_reverse_iterator it = sites.crbegin(); it != sites.crend() - 1; it++){
            
            if(function(*it, *(it + 1))) return;
            
        }
        
    }
          
        
private:
        
    std::vector<_site> sites;
    
};

#endif /* _PATH_H */

