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
        
        if(nodes_product > 0){
            
            free_agent = new int[nodes_product];
            free_step = new int[nodes_product];
            
            for (int i = 0; i < nodes_product; i++) {
                free_agent[i] = NodeType::freeNode;
                free_step[i] = 0;
            }
            
            if(size > 0){            
            
                nodes = new int[size];

            }
            
        }
        
        
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
        
        if(nodes_product > 0){
            
            free_agent = new int[nodes_product];
            free_step = new int[nodes_product];
            
            for (int i = 0; i < nodes_product; i++) {
                free_agent[i] = orig.free_agent[i];
                free_step[i] = orig.free_step[i];
            }
        
            if(size > 0){ 

                this->nodes = new int[size];

                for (unsigned i = 0; i < size; i++)
                    this->nodes[i] = orig.nodes[i];

            }
        
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

    virtual ~_stepMap() {

        if (nodes != nullptr) delete [] nodes;
        if (row_edge != nullptr) delete [] row_edge;
        if (colunm_edge != nullptr) delete [] colunm_edge;
        
        if (free_agent != nullptr) delete [] free_agent;
        if (free_step != nullptr) delete [] free_step;

    }
    
    void stepView(const _stepPath& path)const{
        
        stepView(path.currentSite().GetStep(), path.goalSite().GetStep());
        
    }
    
    void stepView(unsigned from, unsigned to)const{
        
        for (int i = 0; i < to - from + 1; i++) {
            
            stepView(from + i);

        }
        
    }
        
    void stepView(unsigned step)const{
        
        unsigned p = step * nodes_product;
        
        std::cout << "step: " << step << std::endl;
        
        for (int r = 0; r < row_size; r++) {
            
            for (int c = 0; c < colunm_size; c++) {
                
                unsigned index = r * colunm_size + c;
                
                int nodotype = free_agent[index];
                
//                 || nodotype != NodeType::freeNode
                
                if(nodotype == NodeType::freeNode || step < free_step[index]){
                    
                    nodotype = nodes[p + index];
                    
                }               
                
                if(nodotype < 0){
                
                    std::cout << nodotype << " ";
                
                } else {
                    
                    std::cout << " " << nodotype << " ";
                    
                }

            }
            
            std::cout << std::endl;

        }
        
        std::cout << std::endl;

        
    }
    
    void free_agent_view()const{
        
        std::cout << "free_agent" << std::endl;
        
        for (int r = 0; r < row_size; r++) {
            
            for (int c = 0; c < colunm_size; c++) {
                
                unsigned index = r * colunm_size + c;
                
                if(free_agent[index] < 0){
                
                    std::cout << free_agent[index] << " ";
                
                } else {
                    
                    std::cout << " " << free_agent[index] << " ";
                    
                }

            }
            
            std::cout << std::endl;

        }
        
        std::cout << std::endl;

        
    }
    
    void free_step_view()const{
        
        std::cout << "free_step" << std::endl;
        
        for (int r = 0; r < row_size; r++) {
            
            for (int c = 0; c < colunm_size; c++) {
                
                unsigned index = r * colunm_size + c;
                
                if(free_step[index] < 0){
                
                    std::cout << free_step[index] << " ";
                
                } else {
                    
                    std::cout << " " << free_step[index] << " ";
                    
                }

            }
            
            std::cout << std::endl;

        }
        
        std::cout << std::endl;

        
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
        
        setNodeType(site.GetStep(), site.GetRow(), site.GetColunm(), type);

    }
    
    void checkRowColunm(unsigned row, unsigned column)const{
        
        if (row < row_size){
            
            if(column < colunm_size){
                
                return;
                
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "invalid colunm: " << column;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }
                
            }
            
        }
        
        try {
            std::ostringstream stream;
            stream << "invalid row: " << row;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }       
        
    }
    
    bool isPathDefinitelyFree(unsigned step, unsigned row, unsigned column, int type) const {
                
//        if (row < row_size && column < colunm_size){
//            
//            for (int s = step; s < this->step_size; s++){
//                
//                int t = this->nodes[s * nodes_product + row * colunm_size + column];
//                
//                if(t != NodeType::freeNode && t != type){
//                    
//                    return false;
//                    
//                }                    
//                
//            }
//            
//        }
//        
//        return true;
        
        /////////////
        
        checkRowColunm(row, column);
        
        unsigned index = row * colunm_size + column;
        
        if(//free_step[index] < step  && 
                (free_agent[index] == NodeType::freeNode || 
                    free_agent[index] == type)){
            
            return true;
            
        }
        
        return false;
        
    }
    
    bool isPathDefinitelyFree(const _stepSite& site, int type) const {
        
        return isPathDefinitelyFree(site.GetStep(), site.GetRow(), site.GetColunm(), type);
        
    }
    
    void setTypesFrom(unsigned fromStep, unsigned row, unsigned column, int from,  int to) {

//        if (row < row_size && column < colunm_size)
//            for (int step = fromStep; step < this->step_size; step++){
//                unsigned index = step * nodes_product + row * colunm_size + column;                
//                if(this->nodes[index] == from)
//                    this->nodes[index] = to;                
//            }
        
        checkRowColunm(row, column);
        
        unsigned index = row * colunm_size + column;
        
        if(free_agent[index] == from || free_agent[index] == NodeType::freeNode){
            
//            if(free_step[index] <= fromStep){
            
            if(to == NodeType::freeNode){
                free_step[index] = 0;
            }else{
                free_step[index] = fromStep;
            }
                
            free_agent[index] = to;

//            }else{
//                
//                stepView(0, fromStep);
//                free_agent_view();
//                free_step_view();
//                
//                try {
//                    std::ostringstream stream;
//                    stream << "invalid step: " << fromStep;
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                } 
//                
//            }
            
        } 
//        else {
//            
//            stepView(0, fromStep);
//            free_agent_view();
//            free_step_view();
//            
//            try {
//                std::ostringstream stream;
//                stream << "invalid type: " << from;
//                MAPD_EXCEPTION(stream.str());
//            } catch (std::exception& e) {
//                std::cout << e.what() << std::endl;
//                std::abort();
//            } 
//            
//        }
        
        
        
        

    }
    
    void resetTypesInStepColunm(unsigned row, unsigned column, int type) {
        
//        if (row < row_size && column < colunm_size){
//        
//            for (int step = 0; step < this->step_size; step++){
//                unsigned index = step * nodes_product + row * colunm_size + column;    
//                this->nodes[index] = type;                
//            }
//        
//        }
        
        checkRowColunm(row, column); 
        
        int step = 0;
        this->nodes[step++ * nodes_product + row * colunm_size + column] = type;
        
        for (; step < this->step_size; step++){
            unsigned index = step * nodes_product + row * colunm_size + column;    
            this->nodes[index] = NodeType::freeNode;                
        }
        
        unsigned index = row * colunm_size + column;
            
        free_step[index] = 0;
        free_agent[index] = type;   
        
        

    }
    
    
    void setTypesFrom(const _stepSite& site, int from,  int to) {

        setTypesFrom(site.GetStep(), site.GetRow(), site.GetColunm(), from, to);

    }
    
    
    void setMoving(const _stepPath& path, int type){
        
        if(!path.empty()){
        
            auto current = path.currentSite();
            this->setTypesFrom(current.GetStep() + 1, current.GetRow(), current.GetColunm(), type, NodeType::freeNode);
            auto goal = path.goalSite();
            this->setTypesFrom(goal.GetStep(), goal.GetRow(), goal.GetColunm(), NodeType::freeNode, type);       


            path.movingList([this,type](const _stepSite& s, const _stepSite& g){

                this->setNodeType(s, type);
                this->setEdgeType(s.GetStep(), s, g, type);

                return false;

            });
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
        
    void deleteMoving(const _stepPath& path, int type){
        
        if(!path.empty()){
        
            auto goal = path.goalSite();
            this->setTypesFrom(goal.GetStep(), goal.GetRow(), goal.GetColunm(), type, NodeType::freeNode);      
            
            path.movingList([this](const _stepSite& s, const _stepSite& g){

                this->setNodeType(s, NodeType::freeNode);
                this->setEdgeType(s.GetStep(), s, g, EdgeType::freeEdge);

                return false;

            });
            
        
        } else {
            
            try {
                std::ostringstream stream;
                stream << "empty path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
    
    virtual bool isNodeBelonging(const _stepSite& site) const {
        
        return site.GetStep() < step_size && site.GetRow() < row_size && site.GetColunm() < colunm_size;
        
    }
    
    virtual void listNeighborFreePaths(const _stepSite& site, int type, const std::function<bool(const _stepSite&)>& function) const {
        
        _stepSite size = _stepSite(step_size, row_size, colunm_size);
                
            site.listNeighbors(size, [this, type, &function, site](const _stepSite& neigh){
                
                unsigned index = neigh.GetRow() * colunm_size + neigh.GetColunm();
                
                int nodotype = free_agent[index];
                
                if(nodotype != NodeType::freeNode && 
                    nodotype != type &&
                        neigh.GetStep() >= free_step[index]){
                    
                    return false;
                    
                } 
                
                unsigned nodes_index = neigh.GetStep() * nodes_product + index;
                    
                nodotype = this->nodes[nodes_index];                                                 

                if((nodotype == type || nodotype == NodeType::freeNode) && this->hasPath(site, neigh)) 
                    if(function(neigh)) return true;

                return false;
                
            });
                
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
        *colunm_edge = nullptr,
            *free_agent = nullptr,
            *free_step = nullptr;
    
    unsigned 
        step_size = 0, 
        colunm_size = 0, 
        row_size = 0, 
        nodes_product = 0,
        row_product = 0,
        colunm_product = 0;
    
};

#endif /* _STEPMAP_H */

