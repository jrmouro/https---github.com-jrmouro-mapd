/* 
 * File:   InstanceTask.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 10:07
 */

#ifndef INSTANCETASK_H
#define INSTANCETASK_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>

#include "TaskMap.h"
#include "SiteMap.h"

class InstanceTask {
public:
    
    friend class InstanceMAPD;

    InstanceTask(unsigned numTask, unsigned totalStep) : numTask(numTask), lastStep(totalStep) { }
    InstanceTask(const InstanceTask& orig) : taskMap(orig.taskMap), numTask(orig.numTask), lastStep(orig.lastStep) { }

    
    virtual ~InstanceTask() { }
    
    static InstanceTask* load(std::string filename_instance, std::function<const _site(unsigned)> endpointOracle) {

        std::ifstream filestream(filename_instance);

        if (filestream.is_open()) {

            unsigned ntask, tstep;
            std::string line;

            getline(filestream, line);
            std::stringstream(line) >> ntask;
            
            getline(filestream, line);
            std::stringstream(line) >> tstep;

            auto itask = new InstanceTask(ntask, tstep);
            
            itask->loadTasks(filestream, endpointOracle);

            return itask;

        }

        std::cout << "Instance task file not found." << std::endl;

        return nullptr;

    }

    friend std::ostream& operator<<(std::ostream& os, const InstanceTask& obj) {
        
        os << obj.taskMap;

        return os;

    }
    
    void loadTasks(std::ifstream& filestream, std::function<const _site(unsigned)> oracle){
        this->taskMap.load(filestream, oracle);
    }
        
    const TaskMap& getTaskMap() const {
        return taskMap;
    }
    
    unsigned getLastStep() const {
        return lastStep;
    }

    
private:

    unsigned lastStep, numTask;
    TaskMap taskMap;

};

#endif /* INSTANCETASK_H */

