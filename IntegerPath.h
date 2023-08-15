/* 
 * File:   IntegerPath.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 12:58
 */

#ifndef INTEGERPATH_H
#define INTEGERPATH_H

#include <vector>
#include <functional>
#include "IntegerSite.h"
#include "MapdException.h"

class IntegerPath : public Drawable{
public:
    
    IntegerPath() {}
    
    IntegerPath(const IntegerSite& site) {
        sites.push_back(site);
    }
    
    IntegerPath(const IntegerPath& orig) : sites(orig.sites) {}
    
    virtual ~IntegerPath() {}

    void add(const IntegerSite& site) {

        this->sites.push_back(site);

    }
    
    void progress(const IntegerSite& site){       
        
        std::vector<IntegerSite>::iterator it = this->sites.begin();
        this->sites.insert(it, site);       
    
    }
    
    void progress(const IntegerPath& path){       
        
        path.rlist([this](const IntegerSite& site){
            
            this->progress(site);
            
            return false;
            
        });        
    
    }
        
    const IntegerSite& goalSite()const{
        
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
    
    const IntegerSite& currentSite() const{
        
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
    
    void move(){
        
        this->pop();
        
    }
    
    IntegerSite pop(){
        
        if(this->sites.size() > 0){     
            IntegerSite site = this->sites.back();
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

    friend std::ostream& operator<<(std::ostream& os, const IntegerPath& obj) {
        for (std::vector<IntegerSite>::const_reverse_iterator it = obj.sites.rbegin(); it != obj.sites.rend(); ++it)
            os << *it << std::endl;
        return os;
    }

    void clear(){
        this->sites.clear();
    }
    
    bool empty(){
        return this->sites.empty();
    }
    
    virtual void draw(const Render& render) const {
        
        for(std::vector<IntegerSite>::const_iterator it = sites.begin(); it != sites.end(); it++){
            
            sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
            shape_point.setPosition(sf::Vector2f(it->GetColunm() * render.GetCell().first, it->GetRow() * render.GetCell().second));
            shape_point.setFillColor(IntegerSite::TypeColorMap.get(it->getType()));
            render.draw(shape_point);
            
        }
        
    }
    
    
    void list(const std::function<bool(const IntegerSite&)>& function) const {
        
        for(std::vector<IntegerSite>::const_iterator it = sites.cbegin(); it != sites.cend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void rlist(const std::function<bool(const IntegerSite&)>& function) const {
        
        for(std::vector<IntegerSite>::const_reverse_iterator it = sites.crbegin(); it != sites.crend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void moveList(const std::function<bool(const IntegerSite&, const IntegerSite&)>& function) const {
        
        for(std::vector<IntegerSite>::const_reverse_iterator it = sites.crbegin(); it != sites.crend() - 1; it++){
            
            if(function(*it, *(it + 1))) return;
            
        }
        
    }
    
    void setSiteValueByIndex(unsigned index, int type){
        
        std::vector<IntegerSite>::iterator it = sites.begin() + index;
        if(it != sites.end()){
            
            it->setType(type);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "invalid site index";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
    }    
        
private:
        
    std::vector<IntegerSite> sites;
    
};

#endif /* INTEGERPATH_H */

