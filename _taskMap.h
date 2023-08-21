/* 
 * File:   _taskMap.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 08:19
 */

#ifndef _TASKMAP_H
#define _TASKMAP_H

#include <map>
#include <vector>
#include <sstream>
#include <functional>
#include "_task.h"

class _taskMap {
public:

    _taskMap() {}

    _taskMap(const _taskMap& orig) : tasks(orig.tasks) {}

    virtual ~_taskMap() {}
    
    void set(unsigned step, const _task& task) {
        
        std::map<unsigned, std::vector<_task>>::iterator it;
        it = this->tasks.find(step);

        if (it != this->tasks.end()) {

            (*it).second.push_back(task);

        } else {
            auto it2 = this->tasks.insert(std::pair<unsigned, std::vector<_task>>(step, std::vector<_task>()));
            it2.first->second.push_back(task);
        }

    }
    
            
    void listTasks(std::function<bool(unsigned, const _task&)> func) const{
        
        std::map<unsigned, std::vector<_task>>::const_iterator it;
        for (it = this->tasks.begin(); it != this->tasks.end(); ++it) {
                      
            std::vector<_task>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                                
                if(func(it->first, *it2)) return;
                
            }            

        }


    }
    
    void listTasksByStep(unsigned step, std::function<bool(const _task&)> func) const{

        std::map<unsigned, std::vector<_task>>::const_iterator it;
        it = this->tasks.find(step);

        if (it != this->tasks.end()) {
            
            std::vector<_task>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                                
                if(func(*it2)) return;
                
            }  
            
        }

    }

    void load(std::ifstream& filestream, std::function<_site(unsigned)> oracle) {

        std::string line;
        
        int taskId = 0;

        while (std::getline(filestream, line)) {
            
            int t, s, g, ts, tg;
            std::stringstream(line) >> t >> s >> g >> ts >> tg;            
            this->set(t, _task(taskId++, oracle(s), oracle(g)));           

        }

    }
    
    friend std::ostream& operator<<(std::ostream& os, const _taskMap& obj) {
        
        std::map<unsigned, std::vector<_task>>::const_iterator it;
        for (it = obj.tasks.begin(); it != obj.tasks.end(); ++it) {
            
            os << it->first << ":" << std::endl;
          
            std::vector<_task>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
                
                os << "\t "<< *it2 << std::endl;
                
            }            

        }
                
        return os;
        
    }


private:

    std::map<unsigned, std::vector<_task>> tasks;

};

#endif /* _TASKMAP_H */

