/* 
 * File:   RenderExperiment.h
 * Author: ronaldo
 *
 * Created on 29 de julho de 2023, 16:14
 */

#ifndef RENDEREXPERIMENT_H
#define RENDEREXPERIMENT_H

#include "Render.h"

class RenderExperiment : public Experiment{
public:
        
    RenderExperiment(std::string title, unsigned rows, unsigned colunms, unsigned cell, bool loop) :
    title(title), rows(rows), colunms(colunms), cell(cell), loop(loop) { }
    
    RenderExperiment(const RenderExperiment& other) :
    title(other.title), rows(other.rows), colunms(other.colunms), cell(other.cell), loop(other.loop) { }
    
    virtual void reset(){ }
    
    virtual void run(){
        
        if(this->render == nullptr){
                
            this->render = new Render(
                    std::pair<unsigned, unsigned>(this->cell,this->cell),
                    std::pair<unsigned, unsigned>(this->colunms,this->rows),
                    this->title);

            if(this->loop)
                render.loop();     
        
        }
        
    }
           
    virtual ~RenderExperiment(){
        
        if(this->render != nullptr)
            delete this->render;
    
    }        
    
private:
    
    Render *render = nullptr;
    std::string title = "titleless";
    unsigned rows = 21, colunms = 35, cell = 15;
    bool loop = true;

};

#endif /* RENDEREXPERIMENT_H */

