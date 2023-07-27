/* 
 * File:   Task.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 06:58
 */

#ifndef TASK_H
#define TASK_H

#include "Identifiable.h"
#include "EndPoint.h"

class Task : public Identifiable<int>{
    
public:
    
    Task(int id, EndPoint pickup, EndPoint delivery) :
    _id(id), pickup(pickup), delivery(delivery) {}

    Task(const Task& other) :
            _id(other._id),
            pickup(other.pickup), 
            delivery(other.delivery) { }

    virtual ~Task(){}
    
    virtual int id(){
        return this->_id;
    }
    
    EndPoint getDelivery() const {
        return delivery;
    }

    EndPoint getPickup() const {
        return pickup;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Task& obj) {
        os << obj._id << "["<< obj.pickup << " -> " << obj.delivery << "]";
        return os;
    }
    
private:
    
    int _id;
    EndPoint pickup, delivery;

};

#endif /* TASK_H */

