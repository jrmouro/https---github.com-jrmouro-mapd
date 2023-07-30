/* 
 * File:   InstanceMapExperiment.h
 * Author: ronaldo
 *
 * Created on 29 de julho de 2023, 08:20
 */

#ifndef INSTANCEMAPEXPERIMENT_H
#define INSTANCEMAPEXPERIMENT_H

#include "Experiment.h"
#include "InstanceMap.h"

class InstanceMapExperiment : public Experiment{
public:
    
    InstanceMapExperiment(std::string filename, bool render) : filename(filename), render(render) { }

    InstanceMapExperiment(const InstanceMapExperiment& other) :
    instanceMap(other.instanceMap), filename(other.filename), render(other.render) {
    
        if(other.instanceMap != nullptr){
            
            this->instanceMap = InstanceMap::load(other.filename);
            
        } else {
            
            this->instanceMap = nullptr;
            
        }
        
    }

    
    virtual void run(){
        
        if(this->instanceMap == nullptr){      
            
            this->instanceMap = InstanceMap::load(this->filename);
            
        }
        
        if(this->render){
        
            Render render(
                    std::pair<unsigned, unsigned>(16,16),
                    std::pair<unsigned, unsigned>(this->instanceMap->getColumn_size(),this->instanceMap->getRow_size()),
                    this->filename, this->instanceMap);

            render.loop();
        
        }
        
    }
    
    virtual ~InstanceMapExperiment(){
        
        if(this->instanceMap != nullptr){
            delete this->instanceMap;
            this->instanceMap = nullptr;
        }
        
    }
    
protected:
    InstanceMap *instanceMap = nullptr;
    std::string filename;
    
private:   
    
    bool render = false;
};

#endif /* INSTANCEMAPEXPERIMENT_H */

