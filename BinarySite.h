/* 
 * File:   BinarySite.h
 * Author: ronaldo
 *
 * Created on 5 de agosto de 2023, 16:45
 */

#ifndef BINARYSITE_H
#define BINARYSITE_H

#include "Drawable.h"

class BinarySite : public Drawable{
public:   
    
    static const class _TypeColorMap{
        
    public:
                
        sf::Color get(bool value) const {
            if(value) return sf::Color::White;
            return sf::Color::Black;
        }    
        
    } TypeColorMap;
        
    BinarySite() : _step(0), _row(0), _colunm(0), value(false) {}
    
    BinarySite(unsigned row, unsigned colunm) : _step(0), _row(row), _colunm(colunm), value(false) {}
    
    BinarySite(unsigned step, unsigned row, unsigned colunm) : _step(step), _row(row), _colunm(colunm), value(false) {}
    
    BinarySite(unsigned row, unsigned colunm, bool value) : _step(0), _row(row), _colunm(colunm), value(value) {}
    
    BinarySite(unsigned step, unsigned row, unsigned colunm, bool value) : _step(step), _row(row), _colunm(colunm), value(value) {}

    BinarySite(const BinarySite& orig) : _step(orig._step), _row(orig._row), _colunm(orig._colunm), value(orig.value) {}

    virtual ~BinarySite() { }

    unsigned colunm() const {
        return _colunm;
    }

    unsigned row() const {
        return _row;
    }
    
    unsigned step() const {
        return _step;
    }
    
    bool getValue() const {
        return value;
    }

    void setValue(bool value) {
        this->value = value;
    }

    friend std::ostream& operator<<(std::ostream& os, const BinarySite& obj) {
        os << obj._step << " . " << obj._row << " . " << obj._colunm << "[" << obj.value << "]";
        return os;
    }
    
    virtual void draw(const Render& render) const {
        
        sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
        shape_point.setPosition(sf::Vector2f(this->_colunm * render.GetCell().first, this->_row * render.GetCell().second));
        shape_point.setFillColor(BinarySite::TypeColorMap.get(this->value));
        render.draw(shape_point);
        
    }
    
    virtual bool match(const BinarySite& other) const{
        
        return this->_row == other._row && this->_colunm == other._colunm;
        
    }
    

private:
    unsigned _step, _row, _colunm;
    bool value;

};

#endif /* BINARYSITE_H */

