/* 
 * File:   Path.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:36
 */

#ifndef PATH_H
#define PATH_H

#include <vector>
#include "Site.h"
#include "MapdException.h"

class Path : public Drawable{
public:

    Path() {}

    Path(const Path& orig) : sites(orig.sites) {}

    virtual ~Path() {}

    void set(unsigned index, const Site& site) {

        if (index < this->sites.size()) {

            if (index > 0) {

                for (std::vector<Site>::const_iterator it = this->sites.begin() + index; it != this->sites.end(); ++it) {

                    this->d_row -= std::abs((int) (*(it - 1)).row() - (int) (*it).row());
                    this->d_colunm -= std::abs((int) (*(it - 1)).colunm() - (int) (*it).colunm());
                                        
                }

            } else {

                this->d_row = 0;
                this->d_colunm = 0;

            }

            this->sites.erase(this->sites.begin() + index, this->sites.end());

        } else {

            try {
                std::ostringstream stream;
                stream << "invalid index [" << index << "]";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

        this->add(site);

    }

    void add(const Site& site) {

        if (this->sites.size() > 0) {

            Site lastSite = this->sites[this->sites.size() - 1];

            int rowDiff = std::abs((int) lastSite.row() - (int) site.row());
            int colDiff = std::abs((int) lastSite.colunm() - (int) site.colunm());

            if (!((rowDiff + colDiff) == 1)) {

                try {
                    std::ostringstream stream;
                    stream << "invalid path [" << site << "]";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            } else {

                this->d_row += rowDiff;
                this->d_colunm += colDiff;

            }

        }

        this->sites.push_back(site);

    }

    friend std::ostream& operator<<(std::ostream& os, const Path& obj) {
        for (std::vector<Site>::const_iterator it = obj.sites.begin(); it != obj.sites.end(); ++it)
            os << *it << std::endl;
        os << obj.d_row << " . " << obj.d_colunm;
        return os;
    }
    
    unsigned distance_colunm() const {
        return d_colunm;
    }

    unsigned distance_row() const {
        return d_row;
    }

    void clear(){
        this->sites.clear();
        this->d_row = 0;
        this->d_colunm = 0;
    }
    
    virtual void draw(const Render& render) const {
        
        for(std::vector<Site>::const_iterator it = sites.begin(); it != sites.end(); it++){
            
            it->draw(render);
            
        }
        
    }

private:
    std::vector<Site> sites;
    unsigned d_row = 0, d_colunm = 0;
};

#endif /* PATH_H */

