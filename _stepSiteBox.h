/* 
 * File:   _stepSiteBox.h
 * Author: ronaldo
 *
 * Created on 21 de novembro de 2023, 10:39
 */

#ifndef _STEPSITEBOX_H
#define _STEPSITEBOX_H

#include "_stepSite.h"
#include "_siteBox.h"


class _stepSiteBox {
public:
    _stepSiteBox(const _stepSite& A, const _stepSite& B){
    
        if(A.GetStep() < B.GetStep()){
            
            this->A = A;
            this->B = B;
            
        } else {
            
            this->A = B;
            this->B = A;
            
        }
    }
    
    _stepSiteBox(const _stepSiteBox& other) :  A(other.A), B(other.B) { }

    virtual ~_stepSiteBox(){}
    
    bool colideWith(const _stepSiteBox& other){
        
        if( A.GetStep() + (B.GetStep() -  A.GetStep()) < other.A.GetStep()
            || A.GetStep() > other.A.GetStep() + (other.B.GetStep() -  other.A.GetStep())){
            return false;
        }
        
        return _siteBox(A, B).colideWith(_siteBox(other.A, other.B));
        
    }    
    
private:
    
    _stepSite A, B;
    
};

#endif /* _STEPSITEBOX_H */

