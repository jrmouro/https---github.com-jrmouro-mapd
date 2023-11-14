/* 
 * File:   _site.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:25
 */

#ifndef _SITE_H
#define _SITE_H

#include <cmath>
#include <iostream>
#include <functional>

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
    
    bool operator==(const _site& right) const {
        return this->match(right);
    }
    
    bool operator>(const _site& right) const {
        
        return right < *this;
        
    }
    
    bool operator<(const _site& right) const {
        
        bool ret = this->row < right.row || (this->row == right.row && this->colunm < right.colunm);
        
        return ret;
        
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
    
    virtual void listNeighbors(const std::function<bool(const _site& site)> function) const {
                        
        int neighbor_row = (int)this->row - 1;
        if(neighbor_row > -1){
            if(function(_site(neighbor_row, this->colunm))) return;
        }
        if(function(_site(this->row + 1, this->colunm))) return;


        int neighbor_colunm = (int)this->colunm - 1;
        if(neighbor_colunm > -1 ){
            if(function(_site(this->row, neighbor_colunm))) return;
        }
        if(function(_site(this->row, this->colunm + 1))) return;

        function(_site(this->row, this->colunm));         
        
        
    }
    
    unsigned linearIndex(unsigned colunm_size) const{
        return row * colunm_size + colunm;
    }
    
    void SetColunm(unsigned colunm) {
        this->colunm = colunm;
    }

    void SetRow(unsigned row) {
        this->row = row;
    }
    
    unsigned siteBoxArea(const _site& other){
        
        if(row < other.row){
            
            if(colunm < other.colunm){
                
                return (other.row - row)*(other.colunm - colunm);
                
            } else {
                
                return (other.row - row)*(colunm - other.colunm);
                
            }
            
        } 
        
        if(colunm < other.colunm){
            
            return (row - other.row)*(other.colunm - colunm); 
            
        }
        
        return (row - other.row)*(colunm - other.colunm);
        
    }
    
    bool insideSiteBox(const _site& s1, const _site& s2){
        
        if(s1.row < s2.row){
            
            if(s1.colunm < s2.colunm){
                
                return row >= s1.row && row <= s2.row && colunm >= s1.colunm && colunm <= s2.colunm;
                
            }else{
                
                return row >= s1.row && row <= s2.row && colunm >= s2.colunm && colunm <= s1.colunm;
                
            }
        }
        
        if(s1.colunm < s2.colunm){
                
            return row >= s2.row && row <= s1.row && colunm >= s1.colunm && colunm <= s2.colunm;

        }

        return row >= s2.row && row <= s1.row && colunm >= s2.colunm && colunm <= s1.colunm;        
        
    }
        
protected:
    unsigned row, colunm;
};

#endif /* _SITE_H */

