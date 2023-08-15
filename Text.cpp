#include "Text.h"
#include "Render.h"

void Text::draw(const Render& render) const{        
    sf::Text text;
    text.setFont(font);
    text.setString(drawText);
    text.setCharacterSize(size.x);
    text.setFillColor(color);
    text.setPosition(position);
    render.draw(text);        
}
