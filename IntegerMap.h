/* 
 * File:   IntegerMap.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 13:04
 */

#ifndef INTEGERMAP_H
#define INTEGERMAP_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <set>

#include "_stepPath.h"
#include "IntegerSite.h"


class IntegerMap{
public:
    
        
    IntegerMap() : step_size(0), colunm_size(0), row_size(0), site_matrix(nullptr) { }

    IntegerMap(unsigned step_size, unsigned row_size, unsigned colunm_size) :  step_size(step_size), colunm_size(colunm_size), row_size(row_size), row_Colunm_product(row_size * colunm_size) {

        unsigned size = step_size * row_Colunm_product;
        
        if(size > 0){
        
            site_matrix = new int[size];
        
        }

    }

    IntegerMap(const IntegerMap& orig) : step_size(orig.step_size), colunm_size(orig.colunm_size), row_size(orig.row_size), row_Colunm_product(orig.row_Colunm_product) {

        unsigned size = orig.step_size * row_Colunm_product;
        
        if(size > 0){ 

            this->site_matrix = new int[size];

            for (unsigned i = 0; i < size; i++)
                this->site_matrix[i] = orig.site_matrix[i];
        
        }
        
    }
    
    IntegerMap& operator=(const IntegerMap& right) {
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
            this->site_matrix = new int[size];
            for (unsigned i = 0; i < size; i++)
                this->site_matrix[i] = right.site_matrix[i];
        }
        return *this;
    }


    virtual ~IntegerMap() {

        if (site_matrix != nullptr) delete site_matrix;

    }
    
    void setType(unsigned linearLocation, int value) {
        
        if (linearLocation < step_size * row_Colunm_product)
            this->site_matrix[linearLocation] = value;

    }

    void setType(unsigned step, unsigned row, unsigned column, int type) {

        if (step < step_size && row < row_size && column < colunm_size)
            this->site_matrix[step * row_Colunm_product + row * colunm_size + column] = type;

    }
    
    void setTypesFrom(unsigned fromStep, unsigned row, unsigned column, int type) {

        if (row < row_size && column < colunm_size)
            for (int step = fromStep; step < this->step_size; step++)
                this->site_matrix[step * row_Colunm_product + row * colunm_size + column] = type;

    }
    
    void setMoving(_stepPath& path, int type){
        
        auto current = path.currentSite();
        this->setTypesFrom(current.GetStep() + 1, current.GetRow(), current.GetColunm(), IntegerSite::Type::free);
        auto goal = path.goalSite();
        this->setTypesFrom(goal.GetStep(), goal.GetRow(), goal.GetColunm(), type);
        path.list([this,type](const _stepSite& site){
            this->setType(site.GetStep(), site.GetRow(), site.GetColunm(), type);
            return false;
        });
        
    }

    int getType(int step, int row, int colunm) const {

        if (step > -1 && step < step_size && row > -1 && row < row_size && colunm > -1 && colunm < colunm_size)
            return this->site_matrix[step * row_Colunm_product + row * colunm_size + colunm];

        return IntegerSite::blocked;

    }
        
    friend std::ostream& operator<<(std::ostream& os, const IntegerMap& obj) {
        
        os << "step_size: " << obj.step_size << std::endl;
        os << "row_size: " << obj.row_size << std::endl;
        os << "colunm_size: " << obj.colunm_size << std::endl;
        
        for (unsigned s = 0; s < obj.step_size; s++) {
            
            os << "step: " << s << std::endl;

            for (unsigned r = 0; r < obj.row_size; r++) {

                for (unsigned c = 0; c < obj.colunm_size; c++) {

                    auto p = obj.site_matrix[s * obj.row_Colunm_product + r * obj.colunm_size + c];

                    if (p == -1) {
                        os << ".";
                    } else if (p == -2) {
                        os << "X";
                    } else {
                        os << "r";
                    }

                }

                os << std::endl;

            }
            
            os << std::endl;
        
        }

        return os;

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
        
    
    
private:

    int* site_matrix = nullptr;
    unsigned step_size = 0, colunm_size = 0, row_size = 0, row_Colunm_product = 0;
    
};

#endif /* INTEGERMAP_H */

