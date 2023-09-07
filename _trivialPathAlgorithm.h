/* 
 * File:   _trivialPathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 5 de setembro de 2023, 18:16
 */

#ifndef _TRIVIALPATHALGORITHM_H
#define _TRIVIALPATHALGORITHM_H

#include "_agent.h"
#include "_stepSite.h"

class _trivialPathAlgorithm {
public:

    virtual void solve(const _stepSite& site, _stepPath& trivialPath) const {

        trivialPath.clear();
        trivialPath.progress(site);
        _stepSite trivial(site);
        trivial.SetStep(trivial.GetStep() + 1);
        trivialPath.progress(trivial);

    }

};

#endif /* _TRIVIALPATHALGORITHM_H */

