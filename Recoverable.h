/* 
 * File:   Recoverable.h
 * Author: ronaldo
 *
 * Created on 8 de dezembro de 2023, 07:22
 */

#ifndef RECOVERABLE_H
#define RECOVERABLE_H

class Recoverable {
public:
    
    virtual void save() = 0;
    virtual void recover() = 0;

};

#endif /* RECOVERABLE_H */

