/* 
 * File:   EndPoint.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 06:10
 */

#ifndef ENDPOINT_H
#define ENDPOINT_H

#include "Site.h"

class EndPoint : public Site{
public:
    
    EndPoint(int id, unsigned row, unsigned colunm) :
        Site(id, row, colunm, Site::Type::endpoint) {}
    
    EndPoint(const EndPoint& other) :  Site(other) {}    
    
    virtual ~EndPoint(){}
    
    friend std::ostream& operator<<(std::ostream& os, const EndPoint& obj) {
        os << obj.row() << " . " << obj.colunm() << "[" << Site::siteTypeToString(obj.getType()) << "]";
        return os;
    }

    
private:
    
        
};

#endif /* ENDPOINT_H */

