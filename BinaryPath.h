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
    
    void insert(unsigned index, const BinarySite& site){       
        
        std::vector<BinarySite>::iterator it;
        it = this->sites.begin() + index;
        
        if(it != this->sites.end()){
            
            this->sites.insert(it, site);
            
        } else {
            
            this->sites.push_back(site);
            
        }       
    
    }
    
    void erase(unsigned index){
        
        std::vector<BinarySite>::iterator it;
        it = this->sites.begin() + index;
        
        if(it != this->sites.end()){
            
            this->sites.erase(it);
            
        }
        
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
    
    unsigned size()const{
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
            shape_point.setPosition(sf::Vector2f(it->colunm() * render.GetCell().first, it->row() * render.GetCell().second));
            shape_point.setFillColor(BinarySite::TypeColorMap.get(it->getValue()));
            render.draw(shape_point);
            
        }
        
    }
    
    

private:
    
    std::vector<BinarySite> sites;
    
};

#endif /* BINARYPATH_H */

