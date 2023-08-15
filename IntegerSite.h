/* 
 * File:   BinarySite.h
 * Author: ronaldo
 *
 * Created on 5 de agosto de 2023, 16:45
 */

#ifndef BINARYSITE_H
#define BINARYSITE_H

#include "_stepSite.h"
#include "Drawable.h"

class BinarySite : public _stepSite, public Drawable{
public:   
    
    static const class _TypeColorMap{
        
    public:
                
        sf::Color get(bool value) const {
            if(value) return sf::Color::White;
            return sf::Color::Black;
        }    
        
    } TypeColorMap;
            
    BinarySite(unsigned step, unsigned row, unsigned colunm, bool value) : _stepSite(step, row, colunm), value(value) {}

    BinarySite(const BinarySite& orig) : _stepSite(orig), value(orig.value) {}

    virtual ~BinarySite() { }

    bool getValue() const {
        return value;
    }

    void setValue(bool value) {
        this->value = value;
    }

    friend std::ostream& operator<<(std::ostream& os, const BinarySite& obj) {
        os << (_stepSite&)obj << "[" << obj.value << "]";
        return os;
    }
    
    virtual void draw(const Render& render) const {
        
        sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
        shape_point.setPosition(sf::Vector2f(this->_colunm * render.GetCell().first, this->_row * render.GetCell().second));
        shape_point.setFillColor(BinarySite::TypeColorMap.get(this->value));
        render.draw(shape_point);
        
    }
        
    void stepping(){
        this->_step++;
    }
    

private:
    unsigned _step, _row, _colunm;
    bool value;

};

#endif /* BINARYSITE_H */

