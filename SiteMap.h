/* 
 * File:   SiteMap.h
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:33
 */

#ifndef SITEMAP_H
#define SITEMAP_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
#include <set>
#include "Site.h"

class SiteMap : public Drawable{
public:
        
    SiteMap() : SiteMap(0, 0){}

    SiteMap(unsigned row_size, unsigned colunm_size) :  colunm_size(colunm_size), row_size(row_size){

        unsigned size = row_size * colunm_size;
        
        if(size > 0){
        
            site_matrix = new Site::Type[size];
        
        }

    }

    SiteMap(const SiteMap& orig) : colunm_size(orig.colunm_size), row_size(orig.row_size){

        unsigned size = orig.row_size * orig.colunm_size;
        
        if(size > 0){ 

            this->site_matrix = new Site::Type[size];

            for (unsigned i = 0; i < size; i++)
                this->site_matrix[i] = orig.site_matrix[i];
        
        }
        
    }
    
    SiteMap& operator=(const SiteMap& right) {
        // Check for self-assignment!
        if (this == &right) // Same object?
            return *this; // Yes, so skip assignment, and just return *this.
        if(this->site_matrix != nullptr) delete this->site_matrix;
        unsigned size = right.row_size * right.colunm_size;
        this->colunm_size = right.colunm_size;
        this->row_size = right.row_size;
        if(size > 0){            
            this->site_matrix = new Site::Type[size];
            for (unsigned i = 0; i < size; i++)
                this->site_matrix[i] = right.site_matrix[i];
        }
        return *this;
    }


    virtual ~SiteMap() {

        if (site_matrix != nullptr) delete site_matrix;

    }
    
    void setType(unsigned linearLocation, Site::Type value) {
        
        if (linearLocation < row_size * colunm_size)
            this->site_matrix[linearLocation] = value;

    }

    void setType(unsigned row, unsigned column, Site::Type value) {

        if (row < row_size && column < colunm_size)
            this->site_matrix[row * colunm_size + column] = value;

    }

    Site::Type getType(int row, int colunm) const {

        if (row > -1 && row < row_size && colunm > -1 && colunm < colunm_size)
            return this->site_matrix[row * colunm_size + colunm];

        return Site::Type::none;

    }
        
    static unsigned linearLocationBySite(const SiteMap& siteMap, const Site& site){
        return site.GetRow() * siteMap.getColumn_size() + site.GetColunm();
    }

    friend std::ostream& operator<<(std::ostream& os, const SiteMap& obj) {
        
        os << "row_size: " << obj.row_size << std::endl;
        os << "colunm_size: " << obj.colunm_size << std::endl;
        
        for (unsigned r = 0; r < obj.row_size; r++) {

            for (unsigned c = 0; c < obj.colunm_size; c++) {

                auto p = obj.site_matrix[r * obj.colunm_size + c];

                if (p == Site::Type::none) {
                    os << "@";
                } else if (p == Site::Type::bot) {
                    os << "r";
                } else if (p == Site::Type::endpoint) {
                    os << "e";
                }else if (p == Site::Type::task) {
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
                
                this->site_matrix[r * colunm_size + c] = t;
                
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
            
    virtual void draw(const Render& render) const;
        
       
private:

    Site::Type* site_matrix = nullptr;
    unsigned colunm_size = 0, row_size = 0;

};

#endif /* SITEMAP_H */

