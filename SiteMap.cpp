#include "SiteMap.h"
#include "Render.h"
#include "Rectangle.h"

const SiteMap::_TypeColorMap SiteMap::TypeColorMap;

void SiteMap::draw(const Render& render) const {

    for (unsigned r = 0; r < row_size; r++)

        for (unsigned c = 0; c < colunm_size; c++) {

            Rectangle background(

                    sf::Vector2f(
                    c * render.GetCell().first,
                    r * render.GetCell().second),
                    sf::Vector2f(render.GetCell().first, render.GetCell().second),
                    SiteMap::TypeColorMap.get(nodes[r * colunm_size + c]));

            background.draw(render);

        }

}

