/* 
 * File:   InstanceExperiment.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 09:47
 */

#ifndef INSTANCEEXPERIMENT_H
#define INSTANCEEXPERIMENT_H

#include "InstanceMap.h"
#include "InstanceTask.h"

class InstanceExperiment {
    
public:
    InstanceExperiment(const std::string mapFilename, const std::string taskFilename) :
    mapFilename(mapFilename), taskFilename(taskFilename) {}
    
    InstanceExperiment(const InstanceExperiment& other) :
    mapFilename(other.mapFilename), taskFilename(other.taskFilename) {}

    virtual ~InstanceExperiment(){}
    
    void run(){
        auto map = InstanceMap::load(this->mapFilename);
        auto tasks = InstanceTask::load(this->taskFilename, [map](unsigned id){
            return *map->getEndPointById(id);
        });
        
        tasks->getTaskMap()->listTasks([map](unsigned step, const Task& task){
       
            map->getSiteMap()->setType(task.getPickup().row(), task.getPickup().colunm(), Site::Type::task);
            map->getSiteMap()->setType(task.getDelivery().row(), task.getDelivery().colunm(), Site::Type::task);
            
            return false;
        });
        
        sf::RenderWindow window(sf::VideoMode(map->getColumn_size()*10, map->getRow_size()*10), this->mapFilename);

        while (window.isOpen())
        {
            sf::Event event;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            map->draw(window);

            window.display();

        }

        if(map != nullptr){

            delete map;

        }
    
        if(tasks != nullptr){

            delete tasks;

        }
    }
    
private:
    const std::string mapFilename, taskFilename;

};

#endif /* INSTANCEEXPERIMENT_H */

