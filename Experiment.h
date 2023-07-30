/* 
 * File:   Experiment.h
 * Author: ronaldo
 *
 * Created on 28 de julho de 2023, 23:04
 */

#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "Resetable.h"

class Experiment{
public:
    virtual void run() = 0;
};

#endif /* EXPERIMENT_H */

