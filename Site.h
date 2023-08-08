/* 
 * File:   Site.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:25
 */

#ifndef SITE_H
#define SITE_H

#include <cmath>
#include <unordered_map>
#include "MapdException.h"
#include "Identifiable.h"
#include "Drawable.h"
#include "BinarySite.h"
#include "_site.h"

class Site : public _site, public Drawable{
public:
    
    enum Type {
        none,
        path,
        bot,
        endpoint,        
        task,      
        util1,
        util2
    };
    
    static const class _TypeColorMap{
        
    public:
        
        _TypeColorMap() {
            map.insert(std::pair<Site::Type,sf::Color>(Type::bot, sf::Color::Yellow));
            map.insert(std::pair<Site::Type,sf::Color>(Type::endpoint, sf::Color::Blue));
            map.insert(std::pair<Site::Type,sf::Color>(Type::path, sf::Color::Green));
            map.insert(std::pair<Site::Type,sf::Color>(Type::task, sf::Color::Cyan));
            map.insert(std::pair<Site::Type,sf::Color>(Type::none, sf::Color::Red));
            map.insert(std::pair<Site::Type,sf::Color>(Type::util1, sf::Color::Black));
            map.insert(std::pair<Site::Type,sf::Color>(Type::util2, sf::Color::White));
        }
        
        sf::Color get(Site::Type siteType) const {
            std::unordered_map<Site::Type,sf::Color>::const_iterator it;
            it = map.find(siteType);
            if(it != map.end()) return it->second;            
            return sf::Color::Transparent;
        }

    private:
        
        std::unordered_map<Site::Type,sf::Color> map;
        
    } TypeColorMap;
    
    Site() : Site(0, 0, Type::none) {}
        
    Site(unsigned row, unsigned colunm) : Site(row, colunm, Type::none) {}    
    
    Site(unsigned row, unsigned colunm, Type _type) : _site(row, colunm), _type(_type) {}

    Site(const Site& orig) : _site(orig), _type(orig._type) {}

    virtual ~Site() { }

    
    Type getType() const {
        return _type;
    }

    void setType(Type _type) {
        this->_type = _type;
    }

    friend std::ostream& operator<<(std::ostream& os, const Site& obj) {
        os << (_site&)obj << "[" << Site::siteTypeToString(obj._type) << "]";
        return os;
    }
    
    static std::string siteTypeToString(Site::Type siteType){
        
        if (siteType == Site::Type::none) {
            return "@";
        } else if (siteType == Site::Type::bot) {
            return "r";
        } else if (siteType == Site::Type::endpoint) {
            return "e";
        }else if (siteType == Site::Type::task) {
            return "t";
        }
        
        return " ";
        
    }
    
    
    virtual void draw(const Render& render) const {
        
        sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
        shape_point.setPosition(sf::Vector2f(this->colunm * render.GetCell().first, this->row * render.GetCell().second));
        shape_point.setFillColor(Site::TypeColorMap.get(this->_type));
        render.draw(shape_point);
        
    }
    
    
    
    virtual BinarySite parse(unsigned step)const{
        
        return BinarySite(step, row, colunm, !(_type == Type::none));
        
    }
    

private:
    Type _type;

};

#endif /* SITE_H */

