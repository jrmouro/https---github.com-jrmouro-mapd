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
#include <algorithm>
#include <cstring>

#include "_env_map.h"
#include "_stepPath.h"
#include "_stepSite.h"


class _stepMap : public _env_map{
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
            
            free_type = new int[nodes_product];
            max_step = new unsigned[nodes_product];
            
            for (int i = 0; i < nodes_product; i++) {
                free_type[i] = NodeType::freeNode;
                max_step[i] = 0;
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
            
            free_type = new int[nodes_product];
            max_step = new unsigned[nodes_product];
                        
            memcpy(this->free_type, orig.free_type, sizeof(int)*nodes_product);
            memcpy(this->max_step, orig.max_step, sizeof(unsigned)*nodes_product);
        
            if(size > 0){ 

                this->nodes = new int[size];

                memcpy(this->nodes, orig.nodes, sizeof(int)*size);

            }
        
        }
        
        if(row_size > 1){
            
            colunm_product = (row_size - 1) * colunm_size;
            size = step_size * colunm_product;
            
            if(size > 0) {
                
                colunm_edge = new int[size];
            
                memcpy(this->colunm_edge, orig.colunm_edge, sizeof(int)*size);
                
            }
            
        }
        
        if(colunm_size > 1){
            
            row_product = row_size * (colunm_size - 1);
            size = step_size * row_product;
            
            if(size > 0){
                
                row_edge = new int[size];
                
                memcpy(this->row_edge, orig.row_edge, sizeof(int)*size);
                
            }
            
        }
        
    }

    virtual ~_stepMap() {

        if (nodes != nullptr) delete [] nodes;
        if (row_edge != nullptr) delete [] row_edge;
        if (colunm_edge != nullptr) delete [] colunm_edge;
        
        if (free_type != nullptr) delete [] free_type;
        if (max_step != nullptr) delete [] max_step;

    }
    
    virtual void stepView(const _stepPath& path)const{
        
        stepView(path.currentSite().GetStep(), path.goalSite().GetStep());
        
    }
    
    virtual void stepView(unsigned from, unsigned to)const{
        
        for (int i = 0; i < to - from + 1; i++) {
            
            stepView(from + i);

        }
        
    }
        
    virtual void stepView(unsigned step)const{
        
        unsigned p = step * nodes_product;
        
        std::cout << "step: " << step << std::endl;
        
        for (int r = 0; r < row_size; r++) {
            
            for (int c = 0; c < colunm_size; c++) {
                
                unsigned index = r * colunm_size + c;
                
                int nodotype = free_type[index];
                
                if(nodotype == NodeType::freeNode || step < max_step[index]){
                    
                    nodotype = nodes[p + index];
                    
                }       
                
                if(nodotype < -1){
                    
                    std::cout << "# ";
                    
                }else  if(nodotype < 0){
                
                    std::cout << ". ";
                
                } else {
                    
                    std::cout << nodotype << " ";
                    
                }

            }
            
            std::cout << std::endl;

        }
        
        std::cout << std::endl;

        
    }
    
    virtual void free_agent_view()const{
        
        std::cout << "free_agent" << std::endl;
        
        for (int r = 0; r < row_size; r++) {
            
            for (int c = 0; c < colunm_size; c++) {
                
                unsigned index = r * colunm_size + c;
                
                if(free_type[index] < 0){
                
                    std::cout << free_type[index] << " ";
                
                } else {
                    
                    std::cout << " " << free_type[index] << " ";
                    
                }

            }
            
            std::cout << std::endl;

        }
        
        std::cout << std::endl;

        
    }
    
    virtual void max_step_view()const{
        
        std::cout << "max_step" << std::endl;
        
        for (int r = 0; r < row_size; r++) {
            
            for (int c = 0; c < colunm_size; c++) {
                
                unsigned index = r * colunm_size + c;
                
                std::cout << max_step[index] << " ";
                 
            }
            
            std::cout << std::endl;

        }
        
        std::cout << std::endl;

        
    }
    
    virtual void listNeighborFreePaths(const _stepSite& site, int type, const std::function<bool(const _stepSite&)>& function) const {
        
        _stepSite size = _stepSite(step_size, row_size, colunm_size);
                
        site.listNeighbors(size, [this, type, &function, site](const _stepSite& neigh){

            if(isFreeToType( neigh, type) && hasPath(site, neigh)) 
                if(function(neigh)) return true;

            return false;

        });
                
    }
    
    virtual bool isPathDefinitelyFree(const _stepSite& site, int type) const {
        
        return isPathDefinitelyFree(site.GetStep(), site.GetRow(), site.GetColunm(), type);
        
    }
    
    virtual void resetTypesInStepColunm(unsigned row, unsigned column, int type) {
        
        checkRowColunm(0, row, column); 
        
        unsigned index = row * colunm_size + column;
        
        this->nodes[index] = type;
        
        for (int step = 1; step < this->step_size; step++){   
            this->nodes[step * nodes_product + index] = NodeType::freeNode;                
        }
                            
        max_step[index] = 0;
        free_type[index] = type;   
        
    }
    
    
    virtual void setMoving(const _stepPath& path, int type){
        
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
        
    virtual void deleteMoving(const _stepPath& path, int type){
        
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
   

    virtual unsigned getColumn_size() const {
        return colunm_size;
    }

    virtual unsigned getRow_size() const {
        return row_size;
    }
    
    virtual unsigned getStep_size() const {
        return step_size;
    } 
    
    virtual void save(){
                
        try {
            std::ostringstream stream;
            stream << "not implemented";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    virtual void unsave(){
        
        try {
            std::ostringstream stream;
            stream << "not implemented";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    virtual void restore(){
        
        try {
            std::ostringstream stream;
            stream << "not implemented";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
            
private:
    
    enum TypeSaved{ set, del};
    
    bool save_state = false;
    std::vector<std::pair<TypeSaved, std::pair<int, _stepPath>>> saved;

    int *nodes = nullptr, 
        *row_edge = nullptr, 
        *colunm_edge = nullptr,
            *free_type = nullptr;
    
    unsigned
    
            *max_step = nullptr;
    
    unsigned 
        step_size = 0, 
        colunm_size = 0, 
        row_size = 0, 
        nodes_product = 0,
        row_product = 0,
        colunm_product = 0;
    
     bool isPathDefinitelyFree(unsigned step, unsigned row, unsigned column, int type) const {
       
        checkRowColunm(step, row, column);
        
        unsigned index = row * colunm_size + column;
                
        if(max_step[index] < step  && 
                (free_type[index] == NodeType::freeNode || 
                    free_type[index] == type)){
            
            return true;
            
        }
        
        return false;
        
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

        if (step < step_size && row < row_size && column < colunm_size){
            
            unsigned index = row * colunm_size + column;
            
            this->nodes[step * nodes_product + index] = type;
            
            if(type == NodeType::freeNode){
                
                if(max_step[index] <= step){
                 
                    max_step[index] = 0;                
                
                    for (int s = step; s > 0; s--) {

                        if(this->nodes[s * nodes_product + index] != NodeType::freeNode){

                            max_step[index] = s;
                            break;

                        }

                    }
                    
                }
                    
            } else if(step > max_step[index]){
                
                max_step[index] = step;
                
            }
        }

    }
    
    void setNodeType(const _stepSite& site, int type) {
        
        setNodeType(site.GetStep(), site.GetRow(), site.GetColunm(), type);

    }
    
    void checkRowColunm(unsigned step, unsigned row, unsigned column)const{
        
        if(step < step_size){
        
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
                
        } else {
            
            try {
            std::ostringstream stream;
            stream << "invalid step: " << step;
            MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            } 
            
        }
        
    }
    
   
    
    
    
    void setTypesFrom(unsigned fromStep, unsigned row, unsigned column, int from,  int to) {

        checkRowColunm(fromStep, row, column);
        
        unsigned index = row * colunm_size + column;
        
        if(from == NodeType::freeNode){
            
            if(free_type[index] == NodeType::freeNode){
                
                if(to != NodeType::freeNode){
                
                    max_step[index] = fromStep;
                    free_type[index] = to;
                
                } else {
                    
                    try {
                        std::ostringstream stream;
                        stream << "invalid to type: " << from;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }                    
                    
                }
                
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "invalid from type: " << from;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }
                
            }
            
        } else {
            
            if(free_type[index] == from){
                
                if(to != NodeType::freeNode){
                    
                    if(to == from){
                        
                        max_step[index] = fromStep;
                        free_type[index] = from;
                        
                    } else {
                        
                        try {
                            std::ostringstream stream;
                            stream << "invalid from type: " << from;
                            MAPD_EXCEPTION(stream.str());
                        } catch (std::exception& e) {
                            std::cout << e.what() << std::endl;
                            std::abort();
                        }
                        
                    }
                
                } else {
                    
                    free_type[index] = NodeType::freeNode;
                    
                    max_step[index] = 0;
                
                    for (int s = fromStep; s >= 0; s--) {

                        if(this->nodes[s * nodes_product + index] != NodeType::freeNode){

                            max_step[index] = s;
                            break;

                        }

                    }
                    
                }
                
            }     
            
        }

    }
    
    
        
//    void setTypesFrom(const _stepSite& site, int from,  int to) {
//
//        setTypesFrom(site.GetStep(), site.GetRow(), site.GetColunm(), from, to);
//
//    }
    
//    virtual bool isNodeBelonging(const _stepSite& site) const {
//        
//        return site.GetStep() < step_size && site.GetRow() < row_size && site.GetColunm() < colunm_size;
//        
//    }
//    
    bool isFreeToType(const _stepSite& site, int type)const{
        
        unsigned index = site.GetRow() * colunm_size + site.GetColunm();
        
        int nodotype = free_type[index];
                
        if(nodotype == NodeType::freeNode || site.GetStep() < max_step[index]){
            
            unsigned nodes_index = site.GetStep() * nodes_product + index;
            
            nodotype = this->nodes[nodes_index];             
            
        }
        
        return nodotype == NodeType::freeNode || nodotype == type;
        
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
    
};

#endif /* _STEPMAP_H */

