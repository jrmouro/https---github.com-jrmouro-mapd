/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

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

class Site : public Identifiable<int>, public Drawable{
public:
    
    
    
    enum Type {
        bot,
        endpoint,
        path,
        task,        
        none,
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
    
    Site(int id) : _id(id), _row(0), _colunm(0), _type(Type::none) {}
    
    Site(int id, unsigned row, unsigned colunm) : _id(id), _row(row), _colunm(colunm), _type(Type::none) {}

    Site(int id, unsigned row, unsigned colunm, Type _type) : _id(id), _row(row), _colunm(colunm), _type(_type) {}

    Site(const Site& orig) : _id(orig._id), _row(orig._row), _colunm(orig._colunm), _type(orig._type) {}

    virtual ~Site() { }

    unsigned colunm() const {
        return _colunm;
    }

    unsigned row() const {
        return _row;
    }
    
    Type getType() const {
        return _type;
    }

    void setType(Type _type) {
        this->_type = _type;
    }

//    static bool areNeighbors(const Site& s1, const Site& s2) {
//
//        int rowDiff = std::abs((int) s1._row - (int) s2._row);
//        int colDiff = std::abs((int) s1._colunm - (int) s2._colunm);
//
//        return (rowDiff + colDiff) == 1;
//    }
//
//    static Site neighbor(Site::Position position, const Site& site) {
//
//        switch (position) {
//            case Position::right:
//                return Site(site._row + 1, site._colunm);
//            case Position::left:
//                if(site._row > 0)
//                    return Site(site._row - 1, site._colunm);
//                break;
//            case Position::up:
//                if(site._colunm > 0)
//                    return Site(site._row, site._colunm - 1);
//                break;
//            case Position::down:
//                return Site(site._row, site._colunm + 1);
//        }
//
//        try {
//            std::ostringstream stream;
//            stream << "invalid position [" << position << "]";
//            MAPD_EXCEPTION(stream.str());
//        } catch (std::exception& e) {
//            std::cout << e.what() << std::endl;
//            std::abort();
//        }
//        
//    }

    friend std::ostream& operator<<(std::ostream& os, const Site& obj) {
        os << obj._row << " . " << obj._colunm << "[" << Site::siteTypeToString(obj._type) << "]";
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
    
    virtual int id() const{
        return this->_id;
    }
    
    virtual void draw(const Render& render) const {
        
        sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
        shape_point.setPosition(sf::Vector2f(this->_colunm * render.GetCell().first, this->_row * render.GetCell().second));
        shape_point.setFillColor(Site::TypeColorMap.get(this->_type));
        render.draw(shape_point);
        
    }
    
    virtual bool match(const Site& other) const{
        
        return this->_row == other._row && this->_colunm == other._colunm;
        
    }
    

private:
    int _id;
    unsigned _row, _colunm;
    Type _type;

};

const Site::_TypeColorMap Site::TypeColorMap;

#endif /* SITE_H */

