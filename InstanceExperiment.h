/* 
 * File:   InstanceExperiment.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 09:47
 */

#ifndef INSTANCEEXPERIMENT_H
#define INSTANCEEXPERIMENT_H


#include <SFML/Graphics.hpp>
#include "InstanceMap.h"
#include "InstanceTask.h"
#include "Experiment.h"
#include "Render.h"

class InstanceExperiment : public Experiment{
    
public:
    InstanceExperiment(const std::string mapFilename, const std::string taskFilename) :
    mapFilename(mapFilename), taskFilename(taskFilename) {}
    
    InstanceExperiment(const InstanceExperiment& other) :
    mapFilename(other.mapFilename), taskFilename(other.taskFilename) {}

    virtual ~InstanceExperiment(){}
    
    virtual void run(){
        auto map = InstanceMap::load(this->mapFilename);
        auto tasks = InstanceTask::load(this->taskFilename, [map](unsigned id){
            return *map->getEndPointById(id);
        });
        
        tasks->getTaskMap()->listTasks([map](unsigned step, const Task& task){
       
            map->getSiteMap()->setType(task.getPickup().row(), task.getPickup().colunm(), Site::Type::task);
            map->getSiteMap()->setType(task.getDelivery().row(), task.getDelivery().colunm(), Site::Type::task);
            
            return false;
        });
        
        Render render(
                std::pair<unsigned, unsigned>(16,16),
                std::pair<unsigned, unsigned>(map->getColumn_size(),map->getRow_size()),
                this->mapFilename);
        
        render.add(map->getSiteMap());
        
        render.loop();
        
//        sf::RenderWindow window(sf::VideoMode(map->getColumn_size()*16, map->getRow_size()*16), this->mapFilename);
//
//        while (window.isOpen()){
//            
//            sf::Event event;
//
//            while (window.pollEvent(event))
//            {
//                if (event.type == sf::Event::Closed)
//                    window.close();
//            }
//
//            window.clear();
//
//            map->draw(window);
//
//            window.display();
//
//        }

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

