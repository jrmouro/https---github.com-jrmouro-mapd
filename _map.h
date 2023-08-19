/* 
 * File:   _map.h
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:33
 */

#ifndef _MAP_H
#define _MAP_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <set>
#include <SFML/Graphics/Color.hpp>
#include "Drawable.h"
#include "Render.h"
#include "_site.h"
#include "MapdException.h"

class _map : public Drawable{
public:
    
    enum Type {
        none,
        path,
        bot,
        endpoint,        
        task
    };
    
    static const class _TypeColorMap{
        
    public:
        
        _TypeColorMap() {
            map.insert(std::pair<Type,sf::Color>(Type::bot, sf::Color::Yellow));
            map.insert(std::pair<Type,sf::Color>(Type::endpoint, sf::Color::Blue));
            map.insert(std::pair<Type,sf::Color>(Type::path, sf::Color::Green));
            map.insert(std::pair<Type,sf::Color>(Type::task, sf::Color::Cyan));
            map.insert(std::pair<Type,sf::Color>(Type::none, sf::Color::Red));
        }
        
        sf::Color get(Type siteType) const {
            std::unordered_map<Type,sf::Color>::const_iterator it;
            it = map.find(siteType);
            if(it != map.end()) 
                return it->second;            
            return sf::Color::Transparent;
        }

    private:
        
        std::unordered_map<Type,sf::Color> map;
        
    } TypeColorMap;
        
    _map() : _map(0, 0){}

    _map(unsigned row_size, unsigned colunm_size) :  colunm_size(colunm_size), row_size(row_size){

        unsigned size = row_size * colunm_size;
        
        if(size > 0){
        
            nodes = new Type[size];
        
        }

    }

    _map(const _map& orig) : colunm_size(orig.colunm_size), row_size(orig.row_size){

        unsigned size = orig.row_size * orig.colunm_size;
        
        if(size > 0){ 

            this->nodes = new Type[size];

            for (unsigned i = 0; i < size; i++)
                this->nodes[i] = orig.nodes[i];
        
        }
        
    }
    
    _map& operator=(const _map& right) {
        // Check for self-assignment!
        if (this == &right) // Same object?
            return *this; // Yes, so skip assignment, and just return *this.
        if(this->nodes != nullptr) delete this->nodes;
        unsigned size = right.row_size * right.colunm_size;
        this->colunm_size = right.colunm_size;
        this->row_size = right.row_size;
        if(size > 0){            
            this->nodes = new Type[size];
            for (unsigned i = 0; i < size; i++)
                this->nodes[i] = right.nodes[i];
        }
        return *this;
    }


    virtual ~_map() {

        if (nodes != nullptr) delete nodes;

    }
    
    void setType(unsigned linearLocation, Type value) {
        
        if (linearLocation < row_size * colunm_size)
            this->nodes[linearLocation] = value;

    }

    void setType(unsigned row, unsigned column, Type value) {

        if (row < row_size && column < colunm_size)
            this->nodes[row * colunm_size + column] = value;

    }

    Type getType(int row, int colunm) const {

        if (row > -1 && row < row_size && colunm > -1 && colunm < colunm_size)
            return this->nodes[row * colunm_size + colunm];

        return Type::none;

    }
       
    friend std::ostream& operator<<(std::ostream& os, const _map& obj) {
        
        os << "row_size: " << obj.row_size << std::endl;
        os << "colunm_size: " << obj.colunm_size << std::endl;
        
        for (unsigned r = 0; r < obj.row_size; r++) {

            for (unsigned c = 0; c < obj.colunm_size; c++) {

                auto p = obj.nodes[r * obj.colunm_size + c];

                if (p == Type::none) {
                    os << "@";
                } else if (p == Type::bot) {
                    os << "r";
                } else if (p == Type::endpoint) {
                    os << "e";
                }else if (p == Type::task) {
                    os << "t";
                } else {
                    os << " ";
                }

            }

            os << std::endl;

        }

        os << std::endl;
        
        

        return os;

    }

    void load(std::ifstream& filestream, std::function<bool(unsigned, unsigned, Type)> func) {

        for (unsigned r = 0; r < this->row_size; r++) {

            std::string line;
            getline(filestream, line);

            for (unsigned c = 0; c < this->colunm_size; c++) {

                Type t = Type::path;
                
                if (line[c] == '@') {
                    t = Type::none;
                } else if (line[c] == 'r') {
                    t = Type::bot;
                } else if (line[c] == 'e') {
                    t = Type::endpoint;
                } 
                
                this->nodes[r * colunm_size + c] = t;
                
                if(func(r, c, t)) return;

            }

        }

    }

    unsigned getColumn_size() const {
        return colunm_size;
    }

    unsigned getRow_size() const {
        return row_size;
    }
    
    virtual bool isNodeBelonging(const _site& site) const {
        
        return site.GetRow() < row_size && site.GetColunm() < colunm_size;
        
    }
    
    virtual void listNeighborFreeSites(const _site& site, const std::function<bool(const _site&)>& function) const {
        
        if(this->isNodeBelonging(site)){
        
            site.listNeighbors([this, &function](const _site& neigh){

                if(this->isNodeBelonging(neigh)){ 
                
                    unsigned index = neigh.GetRow() * colunm_size + neigh.GetColunm();

                    if(this->nodes[index] != Type::none) 
                        if(function(neigh)) return true;

                } 

                return false;
                
            });
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "invalid site";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
                
    }
            
    virtual void draw(const Render& render) const;
        
       
private:

    Type* nodes = nullptr;
    unsigned colunm_size = 0, row_size = 0;

};

#endif /* _MAP_H */

