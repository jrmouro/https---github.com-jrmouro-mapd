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

    InstanceTask(unsigned numTask, unsigned totalStep) : taskMap(new TaskMap()), numTask(numTask), totalStep(totalStep) { }

    InstanceTask(const InstanceTask& orig) : taskMap(new TaskMap(*orig.taskMap)), numTask(orig.numTask), totalStep(orig.totalStep) { }

    virtual ~InstanceTask() {
    
        delete this->taskMap;
        
    }
    
    static InstanceTask* load(std::string filename_instance, std::function<const EndPoint(unsigned)> endpointOracle) {

        std::ifstream filestream(filename_instance);

        if (filestream.is_open()) {

            unsigned ntask, tstep;
            std::string line;
            getline(filestream, line);
            std::stringstream ss(line);         

            ss >> ntask;
            
            ss.clear();
            getline(filestream, line);
            ss << line;
            ss >> tstep;

            auto itask = new InstanceTask(ntask, tstep);
            
            itask->loadTasks(filestream, endpointOracle);

            return itask;

        }

        std::cout << "Instance task file not found." << std::endl;

        return nullptr;

    }

    friend std::ostream& operator<<(std::ostream& os, const InstanceTask& obj) {
        
        os << *obj.taskMap;

        return os;

    }
    
    void loadTasks(std::ifstream& filestream, std::function<const EndPoint(unsigned)> oracle){
        this->taskMap->load(filestream, oracle);
    }
    
    
    TaskMap* getTaskMap() const {
        return taskMap;
    }
    
private:

    unsigned totalStep, numTask;
    TaskMap *taskMap;

};

#endif /* INSTANCETASK_H */

