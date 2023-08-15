/* 
 * File:   _site.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:25
 */

#ifndef _SITE_H
#define _SITE_H

#include <cmath>


class _site{
    
public:
        
    _site(unsigned row = 0, unsigned colunm = 0) :
    row(row), colunm(colunm) {}
    
    _site(const _site& other) :
    row(other.row), colunm(other.colunm) {
    }
    
    _site& operator=(const _site& right) {
        if (this == &right)
            return *this;
        this->row = right.row;
        this->colunm = right.colunm;
        return *this;
    }

    virtual ~_site(){}
    
    unsigned GetColunm() const {
        return colunm;
    }

    unsigned GetRow() const {
        return row;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const _site& obj) {
        os << "(" << obj.row << " . " << obj.colunm << ")";
        return os;
    }
    
    virtual bool match(const _site& other)const{
        return this->row == other.row && this->colunm == other.colunm;
    }
    
    virtual bool isNeighboorTo(const _site& other)const{
        return (this->row == other.row && std::abs((int)this->colunm - (int)other.colunm) == 1) || 
                (this->colunm == other.colunm && std::abs((int)this->row - (int)other.row) == 1);
    }
    
protected:
    unsigned row, colunm;
};

#endif /* _SITE_H */

