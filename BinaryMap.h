/* 
 * File:   BynaryMap.h
 * Author: ronaldo
 *
 * Created on 5 de agosto de 2023, 16:36
 */

#ifndef BINARYMAP_H
#define BINARYMAP_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <set>

#include "BinarySite.h"


class BinaryMap : public Drawable{
public:
    
    BinaryMap() : step_size(0), colunm_size(0), row_size(0), site_matrix(nullptr) { }

    BinaryMap(unsigned step_size, unsigned row_size, unsigned colunm_size) :  step_size(step_size), colunm_size(colunm_size), row_size(row_size), row_Colunm_product(row_size * colunm_size) {

        unsigned size = step_size * row_Colunm_product;
        
        if(size > 0){
        
            site_matrix = new bool[size];
        
        }

    }

    BinaryMap(const BinaryMap& orig) : step_size(orig.step_size), colunm_size(orig.colunm_size), row_size(orig.row_size), row_Colunm_product(orig.row_Colunm_product) {

        unsigned size = orig.step_size * row_Colunm_product;
        
        if(size > 0){ 

            this->site_matrix = new bool[size];

            for (unsigned i = 0; i < size; i++)
                this->site_matrix[i] = orig.site_matrix[i];
        
        }
        
    }
    
    BinaryMap& operator=(const BinaryMap& right) {
        // Check for self-assignment!
        if (this == &right) // Same object?
            return *this; // Yes, so skip assignment, and just return *this.
        if(this->site_matrix != nullptr) delete this->site_matrix;
        unsigned size = right.step_size * right.row_Colunm_product;
        this->colunm_size = right.colunm_size;
        this->row_size = right.row_size;
        this->step_size = right.step_size;
        this->row_Colunm_product = right.row_Colunm_product;
        if(size > 0){            
            this->site_matrix = new bool[size];
            for (unsigned i = 0; i < size; i++)
                this->site_matrix[i] = right.site_matrix[i];
        }
        return *this;
    }


    virtual ~BinaryMap() {

        if (site_matrix != nullptr) delete site_matrix;

    }
    
    void setValue(unsigned linearLocation, bool value) {
        
        if (linearLocation < step_size * row_Colunm_product)
            this->site_matrix[linearLocation] = value;

    }

    void setValue(unsigned step, unsigned row, unsigned column, bool value) {

        if (step < step_size && row < row_size && column < colunm_size)
            this->site_matrix[step * row_Colunm_product + row * colunm_size + column] = value;

    }
    
    void setValuesFrom(unsigned fromStep, unsigned row, unsigned column, bool value) {

        if (row < row_size && column < colunm_size)
            for (int step = fromStep; step < this->step_size; step++)
                this->site_matrix[step * row_Colunm_product + row * colunm_size + column] = value;

    }

    bool getValue(int step, int row, int colunm) const {

        if (step > -1 && step < step_size && row > -1 && row < row_size && colunm > -1 && colunm < colunm_size)
            return this->site_matrix[step * row_Colunm_product + row * colunm_size + colunm];

        return false;

    }
        
    static unsigned linearLocationBySite(const BinaryMap& binaryMap, const BinarySite& site){
        return site.step() * site.row() * site.colunm() + site.row() * binaryMap.getColumn_size() + site.colunm();
    }

    friend std::ostream& operator<<(std::ostream& os, const BinaryMap& obj) {
        
        os << "step_size: " << obj.step_size << std::endl;
        os << "row_size: " << obj.row_size << std::endl;
        os << "colunm_size: " << obj.colunm_size << std::endl;
        
        for (unsigned s = 0; s < obj.step_size; s++) {
            
            os << "step: " << s << std::endl;

            for (unsigned r = 0; r < obj.row_size; r++) {

                for (unsigned c = 0; c < obj.colunm_size; c++) {

                    auto p = obj.site_matrix[r * obj.colunm_size + c];

                    if (p == true) {
                        os << ".";
                    } else {
                        os << " ";
                    }

                }

                os << std::endl;

            }
            
            os << std::endl;
        
        }

        return os;

    }

    void load(std::ifstream& filestream, std::function<bool(unsigned, unsigned, Site::Type)> func) {

        for (unsigned r = 0; r < this->row_size; r++) {

            std::string line;
            getline(filestream, line);

            for (unsigned c = 0; c < this->colunm_size; c++) {

                Site::Type t = Site::Type::path;
                
                if (line[c] == '@') {
                    t = Site::Type::none;
                } else if (line[c] == 'r') {
                    t = Site::Type::bot;
                } else if (line[c] == 'e') {
                    t = Site::Type::endpoint;
                } 
                
                this->site_matrix[r * colunm_size + c] = !(t == Site::Type::none);
                
                if(func(r, c, t)) return;

            }

        }
        
        for(unsigned s = 1; s < this->step_size; s++){
            
            for (unsigned r = 0; r < this->row_size; r++) {

                for (unsigned c = 0; c < this->colunm_size; c++) {
                    
                    this->site_matrix[s * row_Colunm_product + r * colunm_size + c] = this->site_matrix[r * colunm_size + c];
                             

                }

            }
            
        }

    }

    unsigned getColumn_size() const {
        return colunm_size;
    }

    unsigned getRow_size() const {
        return row_size;
    }
    
    unsigned getStep_size() const {
        return step_size;
    }
        
    virtual void draw(const Render& render) const {
        
        for (unsigned r = 0; r < row_size; r++){

            for (unsigned c = 0; c < colunm_size; c++){
                
                sf::RectangleShape shape_point(sf::Vector2f(render.GetCell().first, render.GetCell().second));
                shape_point.setPosition(sf::Vector2f(c * render.GetCell().first, r * render.GetCell().second));                    
                shape_point.setFillColor(BinarySite::TypeColorMap.get(site_matrix[r * colunm_size + c]));
                render.draw(shape_point);
                
            }
            
        }        
                
    }
    
      
    virtual std::vector<BinarySite> neighborhood(const BinarySite& site) const {
        
        std::vector<BinarySite> ret;
        
        const unsigned step = site.step() + 1;
        const unsigned stepProduct = step * this->row_Colunm_product;
        
        int neighbor_row = (int)site.row() - 1;
        int neighbor_colunm = (int)site.colunm() - 1;
                
        auto value = this->getValue(step, neighbor_row, site.colunm());
        if(value){
            ret.push_back(BinarySite(step, neighbor_row, site.colunm(), value));
        }
            
        neighbor_row = (int)site.row() + 1;
        value = this->getValue(step, neighbor_row, site.colunm());
        if(value){
            ret.push_back(BinarySite(step, neighbor_row, site.colunm(), value));
        }
        
        value = this->getValue(step, site.row(), neighbor_colunm);
        if(value){
            ret.push_back(BinarySite(step, site.row(), neighbor_colunm, value));
        }
            
        neighbor_colunm = (int)site.colunm() + 1;
        value = this->getValue(step, site.row(), neighbor_colunm);
        if(value){
            ret.push_back(BinarySite(step, site.row(), neighbor_colunm, value));
        }
                
        value = this->getValue(step, site.row(), site.colunm());
        ret.push_back(BinarySite(step, site.row(), site.colunm(), value));
        
        return ret;
        
    }
    
private:

    bool* site_matrix = nullptr;
    unsigned step_size = 0, colunm_size = 0, row_size = 0, row_Colunm_product = 0;
    
};

#endif /* BINARYMAP_H */

