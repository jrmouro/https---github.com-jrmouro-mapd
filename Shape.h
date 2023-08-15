/* 
 * File:   Drawable.h
 * Author: ronaldo
 *
 * Created on 28 de julho de 2023, 21:12
 */

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Render.h"

class Drawable {
public:            
    virtual void draw(const Render& render) const = 0;
};

#endif /* DRAWABLE_H */

