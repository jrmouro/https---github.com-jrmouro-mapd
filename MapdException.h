/* 
 * File:   MapdException.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 16:05
 */

#ifndef MAPDEXCEPTION_H
#define MAPDEXCEPTION_H

#define MAPD_EXCEPTION(aMessage) mapd_exception(aMessage, __FILE__, __LINE__) 

#include <exception>
#include <sstream>
#include <iostream>

class MapdException : public std::exception {
public:

    MapdException(const std::string& msg) : msg(msg) {
    }

    virtual const char* what() const throw () {
        return this->msg.c_str();
    }

private:
    const std::string msg;
};



inline void mapd_exception(const std::string aMessage,
                        const char* fileName,
                        const std::size_t lineNumber)
{
   std::ostringstream stream;
   stream << "MAPD_EXCEPTION: " << aMessage << " (" << fileName << ") -> " << lineNumber;
   throw MapdException(stream.str());
}

#endif /* MAPDEXCEPTION_H */

