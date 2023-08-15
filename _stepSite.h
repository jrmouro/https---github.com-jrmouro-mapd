/* 
 * File:   _stepSite.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:25
 */

#ifndef _STEPSITE_H
#define _STEPSITE_H

#include <iostream>
#include <vector>
#include "_site.h"


class _stepSite : public _site{
    
public:
    
    _stepSite() : _stepSite(0,0,0){ }

    _stepSite(unsigned step, unsigned row, unsigned colunm) :
    _site(row, colunm), step(step) {}
    
    _stepSite(const _stepSite& other) : _site(other), step(other.step) { }

    virtual ~_stepSite(){}    
    
    unsigned GetStep() const {
        return step;
    }
    
    void SetStep(unsigned step) {
        this->step = step;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const _stepSite& obj) {
        os << obj.step << " . " << (_site&)obj;
        return os;
    }
    
    virtual std::vector<_stepSite> neighborhood(const _stepSite& size) const {
        
        std::vector<_stepSite> ret;
        
        const unsigned step = this->step + 1;
        
        if(step < size.step){
        
            int neighbor_row = (int)this->row - 1;
            int neighbor_colunm = (int)this->colunm;

            if(neighbor_row > -1 && neighbor_row < size.row && neighbor_colunm < size.colunm){
                ret.push_back(_stepSite(step, neighbor_row, neighbor_colunm));
            }
            
            neighbor_row = (int)this->row + 1;
            
            if(neighbor_row < size.row && neighbor_colunm < size.colunm){
                ret.push_back(_stepSite(step, neighbor_row, neighbor_colunm));
            }

            neighbor_row = (int)this->row;
            neighbor_colunm = (int)this->colunm - 1;
            
            if(neighbor_row < size.row && neighbor_colunm > -1 && neighbor_colunm < size.colunm){
                ret.push_back(_stepSite(step, neighbor_row, neighbor_colunm));
            }
            
            neighbor_colunm = (int)this->colunm + 1;
            
            if(neighbor_row < size.row && neighbor_colunm < size.colunm){
                ret.push_back(_stepSite(step, neighbor_row, neighbor_colunm));
            }
            
            neighbor_row = (int)this->row;
            neighbor_colunm = (int)this->colunm;
            
            if(neighbor_row < size.row && neighbor_colunm < size.colunm){
                ret.push_back(_stepSite(step, neighbor_row, neighbor_colunm));
            }
            
        
        }
        
        return ret;
        
    }
    
protected:
    
    unsigned step;
    
};

#endif /* _STEPSITE_H */

