/* 
 * File:   StepPath.h
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 19:36
 */

#ifndef STEPPATH_H
#define STEPPATH_H

#include <vector>
#include "StepSite.h"
#include "MapdException.h"

class StepPath {
public:

    StepPath() { }

    StepPath(const StepPath& orig) : stepSites(orig.stepSites) { }

    virtual ~StepPath() { }

    void set(unsigned index, const StepSite& stepSite) {

        if (index < this->stepSites.size()) {

            if (index > 0) {

                for (std::vector<StepSite>::const_iterator it = this->stepSites.begin() + index; it != this->stepSites.end(); ++it) {

                    this->d_step -= std::abs((int) (*(it - 1)).step() - (int) (*it).step());
                    this->d_row -= std::abs((int) (*(it - 1)).row() - (int) (*it).row());
                    this->d_colunm -= std::abs((int) (*(it - 1)).colunm() - (int) (*it).colunm());
                                        
                }

            } else {

                this->d_step = 0;
                this->d_row = 0;
                this->d_colunm = 0;

            }

            this->stepSites.erase(this->stepSites.begin() + index, this->stepSites.end());

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

        this->add(stepSite);

    }

    void add(const StepSite& stepSite) {

        if (this->stepSites.size() > 0) {

            StepSite lastSite = this->stepSites[this->stepSites.size() - 1];

            int stepDiff = std::abs((int) lastSite.step() - (int) stepSite.step());
            int rowDiff = std::abs((int) lastSite.row() - (int) stepSite.row());
            int colDiff = std::abs((int) lastSite.colunm() - (int) stepSite.colunm());

            if (!((stepDiff + rowDiff + colDiff) == 1)) {

                try {
                    std::ostringstream stream;
                    stream << "invalid path [" << stepSite << "]";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            } else {

                this->d_step += stepDiff;
                this->d_row += rowDiff;
                this->d_colunm += colDiff;

            }

        }

        this->stepSites.push_back(stepSite);

    }

    friend std::ostream& operator<<(std::ostream& os, const StepPath& obj) {
        for (std::vector<StepSite>::const_iterator it = obj.stepSites.begin(); it != obj.stepSites.end(); ++it)
            os << *it << std::endl;
        os << obj.d_step << " . " << obj.d_row << " . " << obj.d_colunm;
        return os;
    }
    
    unsigned distance_colunm() const {
        return d_colunm;
    }

    unsigned distance_row() const {
        return d_row;
    }

    unsigned distance_step() const {
        return d_step;
    }


private:
    std::vector<StepSite> stepSites;
    unsigned d_step = 0, d_row = 0, d_colunm = 0;
};

#endif /* STEPPATH_H */

