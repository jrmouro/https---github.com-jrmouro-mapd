#include "Site.h"
#include "Render.h"
#include "Rectangle.h"

const Site::_TypeColorMap Site::TypeColorMap;

void Site::draw(const Render& render) const {

    Rectangle background(
            
            sf::Vector2f(
                this->colunm * render.GetCell().first,
                this->row * render.GetCell().second),
            sf::Vector2f(render.GetCell().first, render.GetCell().second),
            Site::TypeColorMap.get(this->_type));

    background.draw(render);

}