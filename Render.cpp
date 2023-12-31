#include "Render.h"
#include "Drawable.h"


Render::Render( const std::pair<unsigned, unsigned>& cell,
        const std::pair<unsigned, unsigned>& grid,
        const std::string& title,
        const Drawable* drawable) : 
    cell(cell), 
    grid(grid), 
    title(title){
    
    this->drawables.push_back(drawable);

}

void Render::add(const Drawable* drawable){
    
    this->drawables.push_back(drawable);
    
}


void Render::loop(unsigned elapse, std::function<void()> update){
    
    window = new sf::RenderWindow(
        sf::VideoMode(
            grid.first*cell.first, 
            grid.second*cell.second), 
        title);
    
    sf::Clock clock;
    sf::Time time = sf::milliseconds(0);
    
        
    while (window->isOpen()){
        
        sf::Event event;

        while (window->pollEvent(event)){
            
            if (event.type == sf::Event::Closed)
                window->close();
            
        }

        window->clear();

        for(std::vector<const Drawable*>::const_iterator it = drawables.begin(); it != drawables.end(); it++){
            
            (*it)->draw(*this);
            
        }

        window->display();
        
//        this->elapsed = clock.restart();
        sf::Time current = clock.restart();
        time += current;
        if(time > sf::milliseconds(elapse)){
            time = time - sf::milliseconds(elapse);
            update();
        }

    }

}

