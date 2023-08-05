/* 
 * File:   Task.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 06:58
 */

#ifndef TASK_H
#define TASK_H

#include "Identifiable.h"

class Task : public Identifiable<int>{
    
public:
    
    Task(int id, Site pickup, Site delivery) :
    _id(id), pickup(pickup), delivery(delivery) {}

    Task(const Task& other) :
            _id(other._id),
            pickup(other.pickup), 
            delivery(other.delivery) { }

    virtual ~Task(){}
    
    virtual int id() const{
        return this->_id;
    }
    
    Site getDelivery() const {
        return delivery;
    }

    Site getPickup() const {
        return pickup;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Task& obj) {
        os << obj._id << "["<< obj.pickup << " -> " << obj.delivery << "]";
        return os;
    }
    
private:
    
    int _id;
    Site pickup, delivery;

};

#endif /* TASK_H */

