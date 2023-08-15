/* 
 * File:   Rectangle.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 04:14
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape{
public:
    
    Rectangle(sf::Vector2f position, sf::Vector2f size, sf::Color color) :
    Shape(position, size, color) { }
    
    Rectangle(const Rectangle& other) : Shape(other) { }

    virtual ~Rectangle(){}
    
    virtual void draw(const Render& render) const;
    
};

#endif /* RECTANGLE_H */

