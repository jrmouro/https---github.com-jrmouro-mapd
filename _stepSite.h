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
#include "_stepPath.h"
#include "MapdException.h"


class _stepSite : public _site{
    
public:
    
    _stepSite() : _stepSite(0,0,0){ }

    _stepSite(unsigned step, unsigned row, unsigned colunm) :
    _site(row, colunm), step(step) {}
    
    _stepSite(const _stepSite& other) : _site(other), step(other.step) { }
    
    _stepSite& operator=(const _stepSite& right) {
        if (this == &right)
            return *this;
        this->step = right.step;
        this->row = right.row;
        this->colunm = right.colunm;
        return *this;
    }

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
        
    virtual void listNeighbors(const _stepSite& size, const std::function<bool(const _stepSite& site)> function) const {
                
        const unsigned step = this->step + 1;
        
        if(step < size.step && this->row < size.row && this->colunm < size.colunm){
                
            int neighbor_row = (int)this->row - 1;
            if(neighbor_row > -1 && neighbor_row < size.row){
                if(function(_stepSite(step, neighbor_row, this->colunm))) return;
            }
            
            neighbor_row = (int)this->row + 1;
            if(neighbor_row < size.row){
                if(function(_stepSite(step, neighbor_row, this->colunm))) return;
            }


            int neighbor_colunm = (int)this->colunm - 1;
            if(neighbor_colunm > -1 && neighbor_colunm < size.colunm){
                if(function(_stepSite(step, this->row, neighbor_colunm))) return;
            }
            
            neighbor_colunm = (int)this->colunm + 1;
            if(neighbor_colunm < size.colunm){
                if(function(_stepSite(step, this->row, neighbor_colunm))) return;
            }

            if(this->row < size.row && this->colunm < size.colunm){
                function(_stepSite(step, this->row, this->colunm));  
            }
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "invalid neighbor site";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }        
        
    }
    
    bool step_match(const _stepSite other) const {
        return step == other.step && match(other);
    }
    
    _stepSite nextSite() const{
        return _stepSite(this->step, this->row, this->colunm);
    }    
    
    unsigned step_distance(const _stepSite other) const {
        if(this->step > other.step){
            return this->step - other.step;
        }
        return other.step - this->step;
    }
    
protected:
    
    unsigned step;
    
};

#endif /* _STEPSITE_H */

