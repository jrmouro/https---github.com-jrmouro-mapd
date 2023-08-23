/* 
 * File:   _c_task.h
 * Author: ronaldo
 *
 * Created on 20 de agosto de 2023, 10:48
 */

#ifndef _C_TASK_H
#define _C_TASK_H

#include "climits"
#include "_task.h"

class _c_task : public _task{
public:
    
    _c_task(int id = 0) : _task(id), concernTaskId(INT_MIN) {}
    
    _c_task(int id, _site pickup, _site delivery, int concernTaskId = INT_MIN) :
    _task(id, pickup, delivery), concernTaskId(concernTaskId) {}
    
    _c_task(const _c_task& other) :
    _task(other), concernTaskId(other.concernTaskId) {}
    
    int GetConcernTaskId() const {
        return concernTaskId;
    }

    virtual ~_c_task(){}
    
    void SetConcernTaskId(int concernTaskId) {
        this->concernTaskId = concernTaskId;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const _c_task& obj) {
        os << (_task&)obj << " concerning: " << obj.concernTaskId;
        return os;
    }

    
private:
    int concernTaskId;
};

#endif /* _C_TASK_H */

