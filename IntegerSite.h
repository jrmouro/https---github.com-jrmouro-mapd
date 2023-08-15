/* 
 * File:   IntegerSite.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 12:45
 */

#ifndef INTEGERSITE_H
#define INTEGERSITE_H

#include "_stepSite.h"
#include "Drawable.h"

class IntegerSite : public _stepSite/*, public Drawable*/{
public:  
    
    enum Type{
        blocked = -2,
        free = -1
    };
    
    static const class _TypeColorMap{
        
    public:
                
        sf::Color get(int type) const {
            if(type < -1) return sf::Color::Black;
            return sf::Color::White;
        }    
        
    } TypeColorMap;
            
    IntegerSite(unsigned step, unsigned row, unsigned colunm, int type) : 
            _stepSite(step, row, colunm), 
            type(type){}

    IntegerSite(const IntegerSite& orig) : _stepSite(orig), type(orig.type) {}
    
    virtual ~IntegerSite() { }

    int getType() const {
        return type;
    }

    void setType(int type) {
        this->type = type;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const IntegerSite& obj) {
        os << (_stepSite&)obj << "[" << obj.type << "]";
        return os;
    }
    
//    virtual void draw(const Render& render) const {
//        
//        sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
//        shape_point.setPosition(sf::Vector2f(this->GetColunm() * render.GetCell().first, this->GetRow() * render.GetCell().second));
//        shape_point.setFillColor(IntegerSite::TypeColorMap.get(this->type));
//        render.draw(shape_point);
//        
//    }
       

private:
    
    int type;

};

#endif /* INTEGERSITE_H */

