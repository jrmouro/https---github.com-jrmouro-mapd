/* 
 * File:   _r_stepMap.cpp
 * Author: ronaldo
 * 
 * Created on 20 de dezembro de 2023, 16:18
 */

#include "_r_stepMap.h"
#include <functional>

#include "_stepSite.h"
#include "_stepPath.h"

#include "./../../../../../NetBeansProjects/Map/_matrix.h"


void _r_stepMap::stepView(const _stepPath& path)const{
        
    stepView(path.currentSite().GetStep(), path.goalSite().GetStep());

}

void _r_stepMap::stepView(unsigned from, unsigned to)const{

    for (int i = 0; i < to - from + 1; i++) {

        stepView(from + i);

    }

}

void _r_stepMap::stepView(unsigned step)const{

    std::cout << "step: " << step << std::endl;
    std::cout << nodes.getMatrix(step) << std::endl;

}

void _r_stepMap::free_agent_view()const{

    std::cout << "free_agent" << std::endl;
    std::cout << free_type << std::endl;
    
}

void _r_stepMap::max_step_view()const{

    std::cout << "max_step" << std::endl;
    std::cout << max_step << std::endl;

}

void _r_stepMap::listNeighborFreePaths(const _stepSite& site, int type, const std::function<bool(const _stepSite&)>& function) const {

    _stepSite size = _stepSite(step_size, row_size, colunm_size);

    site.listNeighbors(size, [this, type, &function, site](const _stepSite& neigh){

        if(isFreeToType( neigh, type) && hasPath(site, neigh)) 
            if(function(neigh)) return true;

        return false;

    });

}

bool _r_stepMap::isPathDefinitelyFree(const _stepSite& site, int type) const {

    return isPathDefinitelyFree(site.GetStep(), site.GetRow(), site.GetColunm(), type);

}

void _r_stepMap::resetTypesInStepColunm(unsigned row, unsigned column, int type) {

    checkRowColunm(0, row, column); 

    this->nodes.set(0,row,column, type);

    for (int step = 1; step < this->step_size; step++){  
        this->nodes.set(step,row,column, NodeType::freeNode);
    }

    max_step.set(row, column, 0);
    free_type.set(row, column, type);   

}


void _r_stepMap::setMoving(const _stepPath& path, int type){

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

void _r_stepMap::deleteMoving(const _stepPath& path, int type){

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

bool _r_stepMap::isPathDefinitelyFree(unsigned step, unsigned row, unsigned column, int type) const {
       
        checkRowColunm(step, row, column);
                        
        if(max_step.get(row, column) < step  && 
                (free_type.get(row, column) == NodeType::freeNode || 
                    free_type.get(row, column) == type)){
            
            return true;
            
        }
        
        return false;
        
    }
    
         
    void _r_stepMap::setRowEdgeType(unsigned step, unsigned row, unsigned column, int type) {
        
        int _size_colunm = colunm_size - 1;

        if (_size_colunm > 1 && step < step_size && row < row_size && column < _size_colunm)
            row_edge.set(step, row, column, type);

    }
    
    int _r_stepMap::getRowEdgeType(unsigned step, unsigned row, unsigned column) const{
        
        int _size_colunm = colunm_size - 1;

        if (_size_colunm > 1 && step < step_size && row < row_size && column < _size_colunm)
            return row_edge.get(step, row, column);
        
        return INT_MAX;

    }
    
    void _r_stepMap::setColunmEdgeType(unsigned step, unsigned row, unsigned column, int type) {
        
        int _size_row = row_size - 1;

        if (_size_row > 1 && step < step_size && row < _size_row && column < colunm_size)
            colunm_edge.set(step, row, column, type);

    }
    
    int _r_stepMap::getColunmEdgeType(unsigned step, unsigned row, unsigned column) const {
        
        int _size_row = row_size - 1;

        if (_size_row > 1 && step < step_size && row < _size_row && column < colunm_size)
            return colunm_edge.get(step, row, column);
        
        return INT_MAX;

    }
    
    int _r_stepMap::edgeType(const _stepSite& s, const _stepSite& g) const{
        
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
    
    bool _r_stepMap::hasPath(const _stepSite& s, const _stepSite& g) const{
        
        auto edgeType = this->edgeType(s, g);
        
        return edgeType == EdgeType::freeEdge;
        
    }
    
    void _r_stepMap::setEdgeType(unsigned step, const _site& s1, const _site& s2, int type){
        
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

    void _r_stepMap::setNodeType(unsigned step, unsigned row, unsigned column, int type) {

        if (step < step_size && row < row_size && column < colunm_size){
            
            this->nodes.set(step, row, column, type);
            
            if(type == NodeType::freeNode){
                
                if(max_step.get(row, column) <= step){
                 
                    max_step.set(row, column, 0);                
                
                    for (int s = step; s > 0; s--) {

                        if(this->nodes.get(step, row, column) != NodeType::freeNode){

                            max_step.set(row, column, s);
                            
                            break;

                        }

                    }
                    
                }
                    
            } else if(step > max_step.get(row, column)){
                
                max_step.set(row, column, step);
                
            }
        }

    }
    
    void _r_stepMap::setNodeType(const _stepSite& site, int type) {
        
        setNodeType(site.GetStep(), site.GetRow(), site.GetColunm(), type);

    }
    
    void _r_stepMap::checkRowColunm(unsigned step, unsigned row, unsigned column)const{
        
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
    
   
    
    
    
    void _r_stepMap::setTypesFrom(unsigned fromStep, unsigned row, unsigned column, int from,  int to) {

        checkRowColunm(fromStep, row, column);
        
        if(from == NodeType::freeNode){
            
            if(free_type.get(row, column) == NodeType::freeNode){
                
                if(to != NodeType::freeNode){
                
                    max_step.set(row, column, fromStep);
                    free_type.set(row, column, to);
                
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
            
            if(free_type.get(row, column) == from){
                
                if(to != NodeType::freeNode){
                    
                    if(to == from){
                        
                        max_step.set(row, column, fromStep);
                        free_type.set(row, column, from);
                        
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
                    
                    free_type.set(row, column, NodeType::freeNode);
                    
                    max_step.set(row, column, 0);
                
                    for (int s = fromStep; s >= 0; s--) {

                        if(this->nodes.get(s, row, column) != NodeType::freeNode){

                            max_step.set(row, column, s);
                            break;

                        }

                    }
                    
                }
                
            }     
            
        }

    }
    
    
        
//    void r_stepMap::setTypesFrom(const _stepSite& site, int from,  int to) {
//
//        setTypesFrom(site.GetStep(), site.GetRow(), site.GetColunm(), from, to);
//
//    }
    
//    bool r_stepMap::isNodeBelonging(const _stepSite& site) const {
//        
//        return site.GetStep() < step_size && site.GetRow() < row_size && site.GetColunm() < colunm_size;
//        
//    }
    
    bool _r_stepMap::isFreeToType(const _stepSite& site, int type)const{
        
        int nodotype = free_type.get(site.GetRow(), site.GetColunm());
                
        if(nodotype == NodeType::freeNode || site.GetStep() < max_step.get(site.GetRow(), site.GetColunm())){
            
            nodotype = this->nodes.get(site.GetStep(),site.GetRow(), site.GetColunm());             
            
        }
        
        return nodotype == NodeType::freeNode || nodotype == type;
        
    }
           
    std::ostream& operator<<(std::ostream& os, const _r_stepMap& obj) {
        
        os << "step_size: " << obj.step_size << std::endl;
        os << "row_size: " << obj.row_size << std::endl;
        os << "colunm_size: " << obj.colunm_size << std::endl;
        
        for (unsigned s = 0; s < obj.step_size; s++) {
            
            os << "step: " << s << std::endl;

            for (unsigned r = 0; r < obj.row_size; r++) {

                for (unsigned c = 0; c < obj.colunm_size; c++) {

                    auto p = obj.nodes.get(s, r, c);

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