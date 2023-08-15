#include "Rectangle.h"
#include "Render.h"

void Rectangle::draw(const Render& render) const {
        
    sf::RectangleShape shape(size);
    shape.setPosition(position);
    shape.setFillColor(color);
    render.draw(shape);

}

