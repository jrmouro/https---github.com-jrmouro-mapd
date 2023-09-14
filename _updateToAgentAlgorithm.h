/* 
 * File:   _updateToAgentAlgorithm.h
 * Author: ronaldo
 *
 * Created on 13 de setembro de 2023, 05:40
 */

#ifndef _UPDATETOAGENTALGORITHM_H
#define _UPDATETOAGENTALGORITHM_H

class _token;
class _agent;
class _updateToAgentAlgorithm {
public:
    virtual bool solve(_token&, _agent&) const = 0;
};

#endif /* _UPDATETOAGENTALGORITHM_H */

