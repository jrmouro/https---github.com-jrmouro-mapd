/* 
 * File:   Task.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 06:58
 */

#ifndef TASK_H
#define TASK_H

#include <iostream>
#include "Identifiable.h"
#include "_site.h"

class Task : public Identifiable<int>{
    
public:
    
    Task() : _id(0) {}
            
    Task(int id, _site pickup, _site delivery) :
    _id(id), pickup(pickup), delivery(delivery) {}

    Task(const Task& other) :
            _id(other._id),
            pickup(other.pickup), 
            delivery(other.delivery) { }
    
    Task& operator=(const Task& right) {
        if (this == &right)
            return *this;
        this->pickup = right.pickup;
        this->delivery = right.delivery;
        this->_id = right._id;
        return *this;
    }


    virtual ~Task(){}
    
    virtual int id() const{
        return this->_id;
    }
    
    _site getDelivery() const {
        return delivery;
    }

    _site getPickup() const {
        return pickup;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Task& obj) {
        os << obj._id << "["<< obj.pickup << " -> " << obj.delivery << "]";
        return os;
    }
    
private:
    
    int _id;
    _site pickup, delivery;

};

#endif /* TASK_H */

