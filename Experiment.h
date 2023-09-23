/* 
 * File:   Experiment.h
 * Author: ronaldo
 *
 * Created on 28 de julho de 2023, 23:04
 */

#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "Identifiable.h"

template<class T>
class Experiment : public Identifiable<T>{
public:
    Experiment(const T _id) :  _id(_id) { }
    
    Experiment(const Experiment<T>& other) : _id(other._id) { }

    virtual void run() = 0;
    
    virtual T id() const { return _id; }
    
    private:
        
        const T _id;
};

#endif /* EXPERIMENT_H */

