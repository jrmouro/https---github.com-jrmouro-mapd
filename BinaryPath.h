/* 
 * File:   BinaryPath.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:36
 */

#ifndef BINARYPATH_H
#define BINARYPATH_H

#include <vector>
#include "BinarySite.h"
#include "MapdException.h"

class BinaryPath : public Drawable{
public:

    BinaryPath() {}
    
    BinaryPath(const BinarySite& site) {
        sites.push_back(site);
    }

    BinaryPath(const BinaryPath& orig) : sites(orig.sites) {}

    virtual ~BinaryPath() {}

//    void set(unsigned index, const Site& site) {
//
//        if (index < this->sites.size()) {
//
//            if (index > 0) {
//
//                for (std::vector<Site>::const_iterator it = this->sites.begin() + index; it != this->sites.end(); ++it) {
//
//                    this->d_row -= std::abs((int) (*(it - 1)).row() - (int) (*it).row());
//                    this->d_colunm -= std::abs((int) (*(it - 1)).colunm() - (int) (*it).colunm());
//                                        
//                }
//
//            } else {
//
//                this->d_row = 0;
//                this->d_colunm = 0;
//
//            }
//
//            this->sites.erase(this->sites.begin() + index, this->sites.end());
//
//        } else if (index > this->sites.size()) {
//            
//            try {
//                std::ostringstream stream;
//                stream << "invalid index [" << index << "]";
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            }
//
//        }
//
//        this->add(site);
//
//    }

    void add(const BinarySite& site) {

        this->sites.push_back(site);

    }
    
    void progress(const BinarySite& site){       
        
        std::vector<BinarySite>::iterator it = this->sites.begin();
        this->sites.insert(it, site);       
    
    }
    
    void progress(const BinaryPath& site){       
        
        this->rlist([this](const BinarySite& site){
            
            this->progress(site);
            
            return false;
            
        });        
    
    }
    
//    void endJoin(const BinaryPath& path){
//        
//    }
    
//    void erase(unsigned index){
//        
//        std::vector<BinarySite>::iterator it;
//        it = this->sites.begin() + index;
//        
//        if(it != this->sites.end()){
//            
//            this->sites.erase(it);
//            
//        }
//        
//    }
    
    const BinarySite& goalSite()const{
        
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
    
    const BinarySite& currentSite() const{
        
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
    
    BinarySite pop(){
        
        if(this->sites.size() > 0){     
            BinarySite site = this->sites.back();
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

    friend std::ostream& operator<<(std::ostream& os, const BinaryPath& obj) {
        for (std::vector<BinarySite>::const_iterator it = obj.sites.begin(); it != obj.sites.end(); ++it)
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
        
        for(std::vector<BinarySite>::const_iterator it = sites.begin(); it != sites.end(); it++){
            
            sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
            shape_point.setPosition(sf::Vector2f(it->GetColunm() * render.GetCell().first, it->GetRow() * render.GetCell().second));
            shape_point.setFillColor(BinarySite::TypeColorMap.get(it->getValue()));
            render.draw(shape_point);
            
        }
        
    }
    
    
    void list(const std::function<bool(const BinarySite&)>& function) const {
        
        for(std::vector<BinarySite>::const_iterator it = sites.cbegin(); it != sites.cend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void rlist(const std::function<bool(const BinarySite&)>& function) const {
        
        for(std::vector<BinarySite>::const_reverse_iterator it = sites.crbegin(); it != sites.crend(); it++){
            
            if(function(*it)) return;
            
        }
        
    }
    
    void moveList(const std::function<bool(const BinarySite&, const BinarySite&)>& function) const {
        
        for(std::vector<BinarySite>::const_reverse_iterator it = sites.crbegin(); it != sites.crend() - 1; it++){
            
            if(function(*it, *(it + 1))) return;
            
        }
        
    }
    
    

private:
    
    std::vector<BinarySite> sites;
    
};

#endif /* BINARYPATH_H */

