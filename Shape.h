/* 
 * File:   Shape.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 4:12
 */

#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Drawable.h"

class Shape: public Drawable {
public: 
    
    Shape(sf::Vector2f position, sf::Vector2f size, sf::Color color) :
    position(position), size(size), color(color) {}
    
    Shape(const Shape& other) :
        position(other.position), size(other.size), color(other.color) { }
    
    virtual ~Shape(){}    
    
    sf::Color getColor() const {
        return color;
    }

    void setColor(sf::Color color) {
        this->color = color;
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    void setPosition(sf::Vector2f position) {
        this->position = position;
    }

    sf::Vector2f getSize() const {
        return size;
    }

    void setSize(sf::Vector2f size) {
        this->size = size;
    }

protected:
    sf::Vector2f position, size;
    sf::Color color;
};

#endif /* DRAWABLE_H */

