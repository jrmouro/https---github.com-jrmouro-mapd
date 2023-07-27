/* 
 * File:   StepTask.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 07:57
 */

#ifndef STEPTASK_H
#define STEPTASK_H

#include "Task.h"

class StepTask : public Task {
public:
    StepTask(unsigned step, unsigned startLocation, unsigned endLocation) :
    Task(startLocation, endLocation), step(step) { }
    StepTask(const StepTask& other) : Task(other), step(other.step) { }
    virtual ~StepTask(){}
private:
    unsigned step;
};

#endif /* STEPTASK_H */

