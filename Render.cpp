#include "Render.h"
#include "Drawable.h"

Render::Render( const std::pair<unsigned, unsigned>& cell,
        const std::pair<unsigned, unsigned>& grid,
        const std::string& title,
        Drawable* drawable) : 
    cell(cell), 
    grid(grid), 
    title(title){
    
    this->drawables.push_back(drawable);

}

void Render::add(Drawable* drawable){
    
    this->drawables.push_back(drawable);
    
}

void Render::loop(){
    
    window = new sf::RenderWindow(
        sf::VideoMode(
            grid.first*cell.first, 
            grid.second*cell.second), 
        title);
    
    sf::Clock clock;
        
    while (window->isOpen()){
        
        this->elapsed = clock.restart();

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

