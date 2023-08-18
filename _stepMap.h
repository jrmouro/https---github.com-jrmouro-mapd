/* 
 * File:   _stepMap.h
 * Author: ronaldo
 *
 * Created on 11 de agosto de 2023, 13:04
 */

#ifndef _STEPMAP_H
#define _STEPMAP_H

#include <cmath>
#include <climits>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <vector>

#include "_stepPath.h"


class _stepMap{
public:
    
    enum NodeType{
        blockedNode = -2,//patern
        freeNode = -1
    };
    
    enum EdgeType{
        blockedEdge = -2,//patern
        freeEdge = -1
    };
    
        
    _stepMap(unsigned step_size, unsigned row_size, unsigned colunm_size) :  
            step_size(step_size), 
            colunm_size(colunm_size), 
            row_size(row_size){
                
        nodes_product = row_size * colunm_size;

        unsigned size = step_size * nodes_product;
        
        if(size > 0) nodes = new int[size];
        
        if(row_size > 1){
            
            colunm_product = (row_size - 1) * colunm_size;
            size = step_size * colunm_product;
            if(size > 0) colunm_edge = new int[size];
            
            for (unsigned i = 0; i < size; i++)
                    this->colunm_edge[i] = EdgeType::freeEdge;
            
        }
        
        if(colunm_size > 1){
            
            row_product = row_size * (colunm_size - 1);
            size = step_size * row_product;
            if(size > 0) row_edge = new int[size];
            
            for (unsigned i = 0; i < size; i++)
                    this->row_edge[i] = EdgeType::freeEdge;
            
        }
        
        

    }

    _stepMap(const _stepMap& orig) : 
        step_size(orig.step_size), 
        colunm_size(orig.colunm_size), 
        row_size(orig.row_size), 
        nodes_product(orig.nodes_product),
        row_product(orig.row_product),
        colunm_product(orig.colunm_product){

        unsigned size = step_size * nodes_product;
        
        if(size > 0){ 

            this->nodes = new int[size];

            for (unsigned i = 0; i < size; i++)
                this->nodes[i] = orig.nodes[i];
        
        }
        
        if(row_size > 1){
            
            colunm_product = (row_size - 1) * colunm_size;
            size = step_size * colunm_product;
            
            if(size > 0) {
                
                colunm_edge = new int[size];
            
                for (unsigned i = 0; i < size; i++)
                    this->colunm_edge[i] = orig.colunm_edge[i];
                
            }
            
        }
        
        if(colunm_size > 1){
            
            row_product = row_size * (colunm_size - 1);
            size = step_size * row_product;
            
            if(size > 0){
                
                row_edge = new int[size];
                
                for (unsigned i = 0; i < size; i++)
                    this->row_edge[i] = orig.row_edge[i];
                
            }
            
        }
        
    }
    
    _stepMap& operator=(const _stepMap& right) {
        // Check for self-assignment!
        if (this == &right) // Same object?
            return *this; // Yes, so skip assignment, and just return *this.
        if(this->nodes != nullptr) delete this->nodes;
        if(this->row_edge != nullptr) delete this->row_edge;
        if(this->colunm_edge != nullptr) delete this->colunm_edge;
        
        this->colunm_size = right.colunm_size;
        this->row_size = right.row_size;
        this->step_size = right.step_size;
        this->nodes_product = right.nodes_product;
        this->row_product = right.row_product;
        this->colunm_product = right.colunm_product;
        
        unsigned size = right.step_size * right.nodes_product;
        
        if(size > 0){            
            this->nodes = new int[size];
            for (unsigned i = 0; i < size; i++)
                this->nodes[i] = right.nodes[i];
        }
        
        if(row_size > 1){
            
            colunm_product = (row_size - 1) * colunm_size;
            size = step_size * colunm_product;
            
            if(size > 0) {
                
                colunm_edge = new int[size];
            
                for (unsigned i = 0; i < size; i++)
                    this->colunm_edge[i] = right.colunm_edge[i];
                
            }
            
        }
        
        if(colunm_size > 1){
            
            row_product = row_size * (colunm_size - 1);
            size = step_size * row_product;
            
            if(size > 0){
                
                row_edge = new int[size];
                
                for (unsigned i = 0; i < size; i++)
                    this->row_edge[i] = right.row_edge[i];
                
            }
            
        }
        
        return *this;
        
    }


    virtual ~_stepMap() {

        if (nodes != nullptr) delete nodes;
        if (row_edge != nullptr) delete row_edge;
        if (colunm_edge != nullptr) delete colunm_edge;

    }
    
    void setRowEdgeType(unsigned step, unsigned row, unsigned column, int type)const {

        int _size_colunm = colunm_size - 1;
        
        if (_size_colunm > 1 && step < step_size && row < row_size && column < _size_colunm)
            this->row_edge[step * row_product + row * _size_colunm + column] = type;

    }
    
    int getRowEdgeType(unsigned step, unsigned row, unsigned column) const{

        int _size_colunm = colunm_size - 1;
        
        if (_size_colunm > 1 && step < step_size && row < row_size && column < _size_colunm)
            return this->row_edge[step * row_product + row * _size_colunm + column];
        
        return INT_MAX;

    }
    
    void setColunmEdgeType(unsigned step, unsigned row, unsigned column, int type) {

        int _size_row = row_size - 1;
        
        if (_size_row > 1 && step < step_size && row < _size_row && column < colunm_size)
            this->colunm_edge[step * colunm_product + _size_row * column + row] = type;

    }
    
    int getColunmEdgeType(unsigned step, unsigned row, unsigned column) const {

        int _size_row = row_size - 1;
        
        if (_size_row > 1 && step < step_size && row < _size_row && column < colunm_size)
            return this->colunm_edge[step * colunm_product + _size_row * column + row];
        
        return INT_MAX;

    }
    
    int edgeType(const _stepSite& s, const _stepSite& g) const{
        
        int diff = (int)g.GetStep() - (int)s.GetStep();
        
        if(diff == 1 && s.GetStep() < step_size && g.GetStep() < step_size){
            
            bool sameRow = s.GetRow() == g.GetRow();
            bool sameColunm = s.GetColunm() == g.GetColunm();
        
            if(sameRow && sameColunm) return EdgeType::freeEdge;
            
            if(sameRow){
                
                diff = std::abs((int)s.GetColunm() - (int)g.GetColunm());
                
                if(diff == 1){
                    
                    unsigned col = std::min<unsigned>(s.GetColunm(), g.GetColunm());
                    
                    
                    return getRowEdgeType(s.GetStep(), s.GetRow(), col);
                    
                }

            } else if(sameColunm){
                
                diff = std::abs((int)s.GetRow() - (int)g.GetRow());
                
                if(diff == 1){
                    
                    unsigned row = std::min<unsigned>(s.GetRow(), g.GetRow());
                    
                    return getColunmEdgeType(s.GetStep(), row, s.GetColunm());
                    
                }

            } 
            
        } 
            
        try {
            std::ostringstream stream;
            stream << "invalid sites path";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
            
        return INT_MAX;
                
    }
    
    bool hasPath(const _stepSite& s, const _stepSite& g) const{
        
        auto edgeType = this->edgeType(s, g);
        
        return edgeType == EdgeType::freeEdge;
        
    }
    
    void setEdgeType(unsigned step, const _site& s1, const _site& s2, int type){
        
        if(step < step_size){
        
            if(s1.GetRow() == s2.GetRow()){
                
                int diff = std::abs((int)s1.GetColunm() - (int)s2.GetColunm());
                
                if(diff == 1){
                    
                    unsigned col = std::min<unsigned>(s1.GetColunm(), s2.GetColunm());
                    
                    setRowEdgeType(step, s1.GetRow(), col, type);
                    
                }
                
                return;

            }else if(s1.GetColunm() == s2.GetColunm()){
                
                int diff = std::abs((int)s1.GetRow() - (int)s2.GetRow());
                
                if(diff == 1){
                    
                    unsigned row = std::min<unsigned>(s1.GetRow(), s2.GetRow());
                    
                    setColunmEdgeType(step, row, s1.GetColunm(), type);
                    
                }
                
                return;

            } 
            
        }
        
        try {
            std::ostringstream stream;
            stream << "invalid edge";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }

    void setNodeType(unsigned step, unsigned row, unsigned column, int type) {

        if (step < step_size && row < row_size && column < colunm_size)
            this->nodes[step * nodes_product + row * colunm_size + column] = type;

    }
    
    void setNodeType(const _stepSite& site, int type) {

        if (site.GetStep() < step_size && site.GetRow() < row_size && site.GetColunm() < colunm_size)
            this->nodes[site.GetStep() * nodes_product + site.GetRow() * colunm_size + site.GetColunm()] = type;

    }
    
    void setTypesFrom(unsigned fromStep, unsigned row, unsigned column, int type) {

        if (row < row_size && column < colunm_size)
            for (int step = fromStep; step < this->step_size; step++)
                this->nodes[step * nodes_product + row * colunm_size + column] = type;

    }
    
    void setTypesFrom(const _stepSite& site, int type) {

        if (site.GetRow() < row_size && site.GetColunm() < colunm_size)
            for (int step = site.GetStep(); step < this->step_size; step++)
                this->nodes[step * nodes_product + site.GetRow() * colunm_size + site.GetColunm()] = type;

    }
    
    void setMoving(_stepPath& path, int type){
        
        auto current = path.currentSite();
        this->setTypesFrom(current.GetStep() + 1, current.GetRow(), current.GetColunm(), NodeType::freeNode);
        auto goal = path.goalSite();
        this->setTypesFrom(goal.GetStep(), goal.GetRow(), goal.GetColunm(), type);
        
        
        
        path.moveList([this,type](const _stepSite& s, const _stepSite& g){
            this->setNodeType(s, type);
            this->setEdgeType(s.GetStep(), s, g, type);
            return false;
        });
        
    }
    
    virtual bool isNodeBelonging(const _stepSite& site) const {
        
        return site.GetStep() < step_size && site.GetRow() < row_size && site.GetColunm() < colunm_size;
        
    }
    
    virtual void listNeighborFreePaths(const _stepSite& site, int type, const std::function<bool(const _stepSite&)>& function) const {
        
        if(this->isNodeBelonging(site)){
        
            site.listNeighbors([this, type, &function, site](const _stepSite& neigh){

                if(this->isNodeBelonging(neigh)){ 
                
                    unsigned index = neigh.GetStep() * nodes_product + neigh.GetRow() * colunm_size + neigh.GetColunm();

                    if((this->nodes[index] == type || this->nodes[index] == NodeType::freeNode) && this->hasPath(site, neigh)) 
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
    
    virtual void listNeighborFreeSites(const _stepSite& site, int type, const std::function<bool(const _stepSite&)>& function) const {
        
        if(this->isNodeBelonging(site)){
        
            site.listNeighbors([this, type, &function](const _stepSite& neigh){

                if(this->isNodeBelonging(neigh)){ 
                
                    unsigned index = neigh.GetStep() * nodes_product + neigh.GetRow() * colunm_size + neigh.GetColunm();

                    if(this->nodes[index] == type || this->nodes[index] == NodeType::freeNode) 
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
    
    
    int nodoType(const _stepSite& site) const {
        
        if(isNodeBelonging(site))
            return this->nodes[site.GetStep() * nodes_product + site.GetRow() * colunm_size + site.GetColunm()];

        return NodeType::blockedNode;

    }

    int nodoType(int step, int row, int colunm) const {

        if (step > -1 && step < step_size && row > -1 && row < row_size && colunm > -1 && colunm < colunm_size)
            return this->nodes[step * nodes_product + row * colunm_size + colunm];

        return NodeType::blockedNode;

    }
        
    friend std::ostream& operator<<(std::ostream& os, const _stepMap& obj) {
        
        os << "step_size: " << obj.step_size << std::endl;
        os << "row_size: " << obj.row_size << std::endl;
        os << "colunm_size: " << obj.colunm_size << std::endl;
        
        for (unsigned s = 0; s < obj.step_size; s++) {
            
            os << "step: " << s << std::endl;

            for (unsigned r = 0; r < obj.row_size; r++) {

                for (unsigned c = 0; c < obj.colunm_size; c++) {

                    auto p = obj.nodes[s * obj.nodes_product + r * obj.colunm_size + c];

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

    int *nodes = nullptr, 
        *row_edge = nullptr, 
        *colunm_edge = nullptr;
    
    unsigned 
        step_size = 0, 
        colunm_size = 0, 
        row_size = 0, 
        nodes_product = 0,
        row_product = 0,
        colunm_product = 0;
    
};

#endif /* _STEPMAP_H */
