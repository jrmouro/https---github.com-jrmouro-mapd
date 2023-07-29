/* 
 * File:   Identifiable.h
 * Author: ronaldo
 *
 * Created on 26 de julho de 2023, 15:28
 */

#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

template<class T>
class Identifiable {
public:
    virtual T id() const = 0;
};

#endif /* IDENTIFIABLE_H */

