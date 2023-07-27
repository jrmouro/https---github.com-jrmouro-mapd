/* 
 * File:   StepMap.h
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 16:38
 */

#ifndef STEPMAP_H
#define STEPMAP_H

#include <exception>
#include <vector>
#include <map>
#include <cstdlib>
#include "SiteMap.h"
#include "MapdException.h"

class StepMap {
public:
    
    StepMap(SiteMap root) {
        this->maps[0] = root;
    }

    StepMap(const StepMap& orig) : maps(orig.maps), key_min(orig.key_min), key_max(orig.key_max) {
    }

    virtual ~StepMap() {
    }

    unsigned expand(unsigned n = 1) {

        for (int i = 0; i < n; i++) {
            maps[key_max + 1] = maps[key_max];
            key_max++;
        }

        return key_max;

    }

    void retract(unsigned n = 1) {
        for (int i = 0; i < n; i++)
            if (maps.size() > 1)
                maps.erase(key_min++);
    }

    void setValue(unsigned step, unsigned row, unsigned column, Site::Type point) {

        if (step <= key_max && step >= key_min){
            maps[step].setType(row, column, point);
        } else {
            try{ 
                std::ostringstream stream;
                stream << "step definition outside the allowed range [" << key_min << " - " << key_max << "]";
                MAPD_EXCEPTION(stream.str()); 
            } catch(std::exception& e){
                std::cout << e.what() << std::endl;
                std::abort();
            }
        }
        
    }

    Site::Type getValue(unsigned step, unsigned row, unsigned column) {

        if (step <= key_max && step >= key_min)
            return maps[step].getType(row, column);

        return Site::Type::none;

    }

    friend std::ostream& operator<<(std::ostream& os, const StepMap& obj) {

        for (auto elem : obj.maps) {

            os << "Map:" << elem.first << std::endl << elem.second << std::endl;

        }

        return os;

    }

private:

    unsigned key_min = 0;
    unsigned key_max = 0;
    std::map<unsigned, SiteMap> maps;

};

#endif /* STEPMAP_H */

