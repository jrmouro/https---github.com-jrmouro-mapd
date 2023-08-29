/* 
 * File:   ReportTaskMap.h
 * Author: ronaldo
 *
 * Created on 24 de agosto de 2023, 05:26
 */

#ifndef REPORTTASKMAP_H
#define REPORTTASKMAP_H

#include <map>
#include "ReportTask.h"

class ReportTaskMap {
    
public:
    
    ReportTaskMap() {}
    
    ReportTaskMap(const ReportTaskMap& other) :
    reportTasks(other.reportTasks) {}

    virtual ~ReportTaskMap(){}
    
    void addTask(const _task& task, int taskArrivalStep){
        this->reportTasks.insert(std::pair<int, ReportTask>(task.id(), ReportTask(task, taskArrivalStep)));
    }
    
    void reportTaskUpdate(const _agent& agent, const _task& task, ReportTask::PathType pathType, const _stepPath& path){
        
        std::map<int, ReportTask>::iterator it = reportTasks.find(task.id());
        
        if(it != reportTasks.end()){
            
            it->second.SetPath(agent, pathType, path);
            
        } else {
            
            try {
                std::ostringstream stream;
                stream << "report task not found";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }
            
        }
        
    }
    
    friend std::ostream& operator<<(std::ostream& os, const ReportTaskMap& obj) {
        for (auto pair : obj.reportTasks) {
            os << "report " << pair.second << std::endl;
        }
        return os;
    }
    
        
    
private:
    
    std::map<int, ReportTask> reportTasks;

};

#endif /* REPORTTASKMAP_H */

