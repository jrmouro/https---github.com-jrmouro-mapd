/* 
 * File:   _r_stepMap.h
 * Author: ronaldo
 *
 * Created on 20 de dezembro de 2023, 16:18
 */

#ifndef _R_STEPMAP_H
#define _R_STEPMAP_H

#include <functional>

#include "./../../../../../NetBeansProjects/Map/_cutrix.h"
#include "_env_map.h"

class _site;
class _stepSite;
class _stepPath;
class _r_stepMap : public _env_map{
public:
    
    enum NodeType{
        blockedNode = -2,//patern
        freeNode = -1
    };
    
    enum EdgeType{
        blockedEdge = -2,//patern
        freeEdge = -1
    };
    
    _r_stepMap(
        unsigned step_size, 
        unsigned row_size, 
        unsigned colunm_size, 
        unsigned grow): 
            nodes(step_size, row_size, colunm_size, grow, NodeType::freeNode),
            row_edge(step_size, row_size, colunm_size - 1, grow, EdgeType::freeEdge),
            colunm_edge(step_size, row_size - 1, colunm_size, grow, EdgeType::freeEdge),
            free_type(row_size, colunm_size, grow, NodeType::freeNode),
            max_step(row_size, colunm_size, grow, 0),
            step_size(step_size), 
            colunm_size(colunm_size), 
            row_size(row_size){}
    
    _r_stepMap(const _r_stepMap& other) :
            nodes(other.nodes), 
            row_edge(other.row_edge), 
            colunm_edge(other.colunm_edge), 
            free_type(other.free_type), 
            max_step(other.max_step), 
            step_size(other.step_size), 
            colunm_size(other.colunm_size), 
            row_size(other.row_size) { }

    
    virtual ~_r_stepMap(){}
    
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
        nodes.save();  
        row_edge.save();
        colunm_edge.save();
        free_type.save();
        max_step.save();        
    }
    
    virtual void unsave(){
        nodes.unsave();
        row_edge.unsave();
        colunm_edge.unsave();
        free_type.unsave();
        max_step.unsave();
    }
    
    virtual void restore(){
        nodes.restore();
        row_edge.restore();
        colunm_edge.restore();
        free_type.restore();
        max_step.restore();
    }
    
    virtual void stepView(const _stepPath& path)const;
    virtual void stepView(unsigned from, unsigned to)const;
    virtual void stepView(unsigned step)const;    
    virtual void free_agent_view()const;    
    virtual void max_step_view()const;
    virtual void listNeighborFreePaths(const _stepSite& site, int type, const std::function<bool(const _stepSite&)>& function) const;
    virtual bool isPathDefinitelyFree(const _stepSite& site, int type) const;    
    virtual void resetTypesInStepColunm(unsigned row, unsigned column, int type);
    virtual void setMoving(const _stepPath& path, int type);        
    virtual void deleteMoving(const _stepPath& path, int type);
    
private:
    
    _cutrix<int> nodes, row_edge, colunm_edge;
    
    _matrix<int> free_type;
    _matrix<unsigned> max_step;
    
    const unsigned 
        step_size = 0, 
        colunm_size = 0, 
        row_size = 0;
    
    bool isPathDefinitelyFree(unsigned step, unsigned row, unsigned column, int type) const;             
    void setRowEdgeType(unsigned step, unsigned row, unsigned column, int type);     
    int getRowEdgeType(unsigned step, unsigned row, unsigned column) const;    
    void setColunmEdgeType(unsigned step, unsigned row, unsigned column, int type);    
    int getColunmEdgeType(unsigned step, unsigned row, unsigned column) const;    
    int edgeType(const _stepSite& s, const _stepSite& g) const;    
    bool hasPath(const _stepSite& s, const _stepSite& g) const;    
    void setEdgeType(unsigned step, const _site& s1, const _site& s2, int type);    
    void setNodeType(unsigned step, unsigned row, unsigned column, int type);    
    void setNodeType(const _stepSite& site, int type);    
    void checkRowColunm(unsigned step, unsigned row, unsigned column)const;      
    void setTypesFrom(unsigned fromStep, unsigned row, unsigned column, int from,  int to);         
//    void setTypesFrom(const _stepSite& site, int from,  int to);     
    bool isFreeToType(const _stepSite& site, int type)const;     
    friend std::ostream& operator<<(std::ostream& os, const _r_stepMap& obj);
};

#endif /* R_STEPMAP_H */

