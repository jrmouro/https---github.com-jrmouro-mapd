/* 
 * File:   TaskMap.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 08:19
 */

#ifndef TASKMAP_H
#define TASKMAP_H

#include <map>
#include <vector>
#include <sstream>
#include <functional>
#include "Task.h"

class TaskMap {
public:

    TaskMap() {}

    TaskMap(const TaskMap& orig) : tasks(orig.tasks) {}

    virtual ~TaskMap() {}
    
    void set(unsigned step, const Task& task) {
        
        std::map<unsigned, std::vector<Task>>::iterator it;
        it = this->tasks.find(step);

        if (it != this->tasks.end()) {

            (*it).second.push_back(task);

        } else {
            auto it2 = this->tasks.insert(std::pair<unsigned, std::vector<Task>>(step, std::vector<Task>()));
            it2.first->second.push_back(task);
        }

    }
    
//    const std::vector<Task>& get(unsigned step){
//        
//        std::map<unsigned, std::vector<Task>>::const_iterator it;
//        it = this->tasks.find(step);
//         if (it != this->tasks.end()) {
//
//            return it->second;
//
//        } 
//        
//        return std::vector<Task>(); 
//    }
            
    void listTasks(std::function<bool(unsigned, const Task&)> func) const{
        
        std::map<unsigned, std::vector<Task>>::const_iterator it;
        for (it = this->tasks.begin(); it != this->tasks.end(); ++it) {
                      
            std::vector<Task>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                                
                if(func(it->first, *it2)) return;
                
            }            

        }


    }
    
    void listTasksByStep(unsigned step, std::function<bool(const Task&)> func) const{

        std::map<unsigned, std::vector<Task>>::const_iterator it;
        it = this->tasks.find(step);

        if (it != this->tasks.end()) {
            
            std::vector<Task>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                                
                if(func(*it2)) return;
                
            }  
            
        }

    }

    void load(std::ifstream& filestream, std::function<_site(unsigned)> oracle) {

        std::string line;
        std::stringstream stream;
        
        int taskId = 0;

        while (std::getline(filestream, line)) {
            
            stream.clear();
            stream << line;
            int t, s, g, ts, tg;
            stream >> t >> s >> g >> ts >> tg;            
            this->set(t, Task(taskId++, oracle(s), oracle(g)));           

        }

    }
    
    friend std::ostream& operator<<(std::ostream& os, const TaskMap& obj) {
        
        std::map<unsigned, std::vector<Task>>::const_iterator it;
        for (it = obj.tasks.begin(); it != obj.tasks.end(); ++it) {
            
            os << it->first << ":" << std::endl;
          
            std::vector<Task>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                
                os << "\t "<< *it2 << std::endl;
                
            }            

        }
                
        return os;
        
    }


private:

    std::map<unsigned, std::vector<Task>> tasks;

};

#endif /* TASKMAP_H */

