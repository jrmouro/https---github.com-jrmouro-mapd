/* 
 * File:   Circle.h
 * Author: ronaldo
 *
 * Created on 14 de agosto de 2023, 04:20
 */

#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"


class Circle : public Shape{
    
public:
    
    Circle(sf::Vector2f position, sf::Vector2f size, sf::Color color) :
    Shape(position, size, color) { }
    
    Circle(const Circle& other) :
    Shape(other) { }

    virtual ~Circle(){}
    
    virtual void draw(const Render& render) const;

};

#endif /* CIRCLE_H */

