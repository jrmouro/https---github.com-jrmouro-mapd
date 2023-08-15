/* 
 * File:   Text.h
 * Author: ronaldo
 *
 * Created on 12 de agosto de 2023, 18:54
 */

#ifndef TEXT_H
#define TEXT_H

//#include <sstream>
//#include <iostream>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Shape.h"
#include "MapdException.h"


class Text : public Shape{
    
public:
    
    Text(std::string drawText, sf::Vector2f position, sf::Vector2f size, sf::Color color) :
    Shape(position, size, color), drawText(drawText) { 
    
        if (!font.loadFromFile("Arial.ttf")){
            try {
                std::ostringstream stream;
                stream << "font not found";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
        }
        
    }
        
    Text(const Text& other) :
    Shape(other), font(other.font), drawText(other.drawText) { }

    virtual ~Text(){}
    
    void setDrawText(std::string drawText) {
        this->drawText = drawText;
    }
      
    virtual void draw(const Render& render) const;
    
private:
    sf::Font font;
    std::string drawText;

};

#endif /* TEXT_H */

