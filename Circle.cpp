#include "Circle.h"
#include "Render.h"
    
void Circle::draw(const Render& render) const{
    sf::CircleShape	shape(size.x);     
    shape.setPosition(position);
    shape.setFillColor(color);
    render.draw(shape);
}


