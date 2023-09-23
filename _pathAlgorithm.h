/* 
 * File:   _pathAlgorithm.h
 * Author: ronaldo
 *
 * Created on 27 de julho de 2023, 16:35
 */

#ifndef _PATHALGORITHM_H
#define _PATHALGORITHM_H

class _map;
class _site;
class _path;
class _pathAlgorithm {
public:
    virtual bool solve(const _map& map, const _site& start, const _site& goal, _path& path) const = 0;
};

#endif /* _PATHALGORITHM_H */

