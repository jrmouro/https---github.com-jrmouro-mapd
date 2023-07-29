#include "Render.h"
#include "Drawable.h"

void Render::add(Drawable* drawable){
    
    this->drawables.push_back(drawable);
    
}

void Render::loop(){
    
    window = new sf::RenderWindow(
        sf::VideoMode(
            grid.first*cell.first, 
            grid.second*cell.second), 
        title);
        
    while (window->isOpen()){

        sf::Event event;

        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        window->clear();

        for(std::vector<Drawable*>::const_iterator it = drawables.begin(); it != drawables.end(); it++){
            
            (*it)->draw(*this);
            
        }

        window->display();

    }

}

