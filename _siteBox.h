/* 
 * File:   _siteBox.h
 * Author: ronaldo
 *
 * Created on 21 de novembro de 2023, 09:32
 */

#ifndef _SITEBOX_H
#define _SITEBOX_H

#include "_site.h"


class _siteBox {
public:
    
    _siteBox(const _site& A, const _site& B) {
        
        if(B.GetRow() < A.GetRow()){
            this->A.SetRow(B.GetRow());
            this->B.SetRow(A.GetRow());            
        } else {
            this->A.SetRow(A.GetRow());
            this->B.SetRow(B.GetRow());
        }
        
        if(B.GetColunm() < A.GetColunm()){
            this->A.SetColunm(B.GetColunm());
            this->B.SetColunm(A.GetColunm());            
        } else {
            this->A.SetColunm(A.GetColunm());
            this->B.SetColunm(B.GetColunm());
        }
    
    }

    _siteBox(const _siteBox& other) : A(other.A), B(other.B) { }
    
    virtual ~_siteBox(){}
        
    bool colideWith(const _siteBox& other){
        
        return checkCollide(
                A.GetRow(), 
                A.GetColunm(), 
                B.GetRow() - A.GetRow(),
                B.GetColunm() - A.GetColunm(),
                other.A.GetRow(), 
                other.A.GetColunm(), 
                other.B.GetRow() - other.A.GetRow(),
                other.B.GetColunm() - other.A.GetColunm());
        
    }
        

private:
    
    _site A, B;
    
    bool checkCollide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    if (y1 + h1 < y2
       || y1 > y2 + h2
       || x1 + w1 < x2
       || x1 > x2 + w2) return false;
    
    return true;
    
  }

};

#endif /* _SITEBOX_H */

