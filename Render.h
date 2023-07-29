/* 
 * File:   Render.h
 * Author: ronaldo
 *
 * Created on 28 de julho de 2023, 21:24
 */

#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>

class Drawable;

class Render {
 public:
        
    Render( const std::pair<unsigned, unsigned>& cell,
            const std::pair<unsigned, unsigned>& grid,
            const std::string& title) : 
        cell(cell), 
        grid(grid), 
        title(title)
//        ,
//        window(
//            new sf::RenderWindow(
//                sf::VideoMode(grid.first*cell.first, grid.second*cell.second), 
//                title))
        {}
            
            
    Render(const Render& other) :
    title(other.title), cell(other.cell), grid(other.grid)
//    , window(new sf::RenderWindow(
//                    sf::VideoMode(other.grid.first*other.cell.first, other.grid.second*other.cell.second))) 
    {}

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
    
//    std::pair<unsigned, unsigned> windowSize() const {
//        return std::pair<unsigned, unsigned>(window->getSize().x, window->getSize().y);
//    }
    
    void loop();
    
    void add(Drawable* drawable);
         
private:
    
    std::string title;
    std::pair<unsigned, unsigned> cell, grid;
    sf::RenderWindow* window;
    std::vector<Drawable*> drawables;

};

#endif /* RENDER_H */

