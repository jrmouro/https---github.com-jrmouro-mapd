/* 
 * File:   _task.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 06:58
 */

#ifndef _TASK_H
#define _TASK_H

#include <iostream>
#include "Identifiable.h"
#include "_site.h"

class _task : public Identifiable<int>{
    
public:
        
    _task(int id = 0) : _id(id), pickup(), delivery() {}
            
    _task(int id, _site pickup, _site delivery) :
    _id(id), pickup(pickup), delivery(delivery) {}

    _task(const _task& other) :
            _id(other._id),
            pickup(other.pickup), 
            delivery(other.delivery) { }
        
    _task& operator=(const _task& right) {
        if (this == &right)
            return *this;
        this->pickup = right.pickup;
        this->delivery = right.delivery;
        this->_id = right._id;
        return *this;
    }


    virtual ~_task(){}
    
    virtual int id() const{
        return this->_id;
    }
    
    void setId(int _id) {
        this->_id = _id;
    }
    
    _site getDelivery() const {
        return delivery;
    }

    _site getPickup() const {
        return pickup;
    }
    
    void setDelivery(_site delivery) {
        this->delivery = delivery;
    }

    void setPickup(_site pickup) {
        this->pickup = pickup;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const _task& obj) {
        os << obj._id << "["<< obj.pickup << " -> " << obj.delivery << "]";
        return os;
    }
    
    bool isInnocuous()const{
        return pickup.match(delivery);
    }
    
private:
    
    int _id;
    _site pickup, delivery;

};

#endif /* _TASK_H */

