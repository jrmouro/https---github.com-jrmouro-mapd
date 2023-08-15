/* 
 * File:   Render.h
 * Author: ronaldo
 *
 * Created on 28 de julho de 2023, 21:24
 */

#ifndef RENDER_H
#define RENDER_H

#include <functional>
#include <SFML/Graphics.hpp>

class Drawable;
class Render {
    
 public:
        
    Render( const std::pair<unsigned, unsigned>& cell,
            const std::pair<unsigned, unsigned>& grid,
            const std::string& title) : 
        cell(cell), 
        grid(grid), 
        title(title){}
        
    Render( const std::pair<unsigned, unsigned>& cell,
        const std::pair<unsigned, unsigned>& grid,
        const std::string& title,
        const std::vector<const Drawable*>& drawables) : 
    cell(cell), 
    grid(grid), 
    title(title),
    drawables(drawables){}
    
    Render( const std::pair<unsigned, unsigned>& cell,
        const std::pair<unsigned, unsigned>& grid,
        const std::string& title,
        const Drawable* drawable);
            
            
    Render(const Render& other) : 
        title(other.title), 
        cell(other.cell), 
        grid(other.grid), 
        drawables(other.drawables){}

    std::pair<unsigned, unsigned> GetCell() const { return cell; }
    std::pair<unsigned, unsigned> GetGrid() const { return grid; }


    virtual ~Render(){
        
        if(this->window != nullptr)

            delete this->window;

    }

    void draw(const sf::Drawable& sf_drawable) const {
        if(this->window != nullptr)
            this->window->draw(sf_drawable);            
    }
    
    void loop(unsigned elapse = 1000, std::function<void()> update = [](){});
    
    void add(const Drawable* drawable);
    
    sf::Time GetElapsed() const {
        return elapsed;
    }
         
private:
    
    std::string title;
    std::pair<unsigned, unsigned> cell, grid;
    sf::RenderWindow* window;
    sf::Time elapsed;
    std::vector<const Drawable*> drawables;

};

#endif /* RENDER_H */

