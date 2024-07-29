#ifndef MAP_H
#define MAP_H

#include <cstdlib>

struct Map {
    static const size_t w = 16;
    static const size_t h = 16;
    static const char map[];
    int get(const size_t i, const size_t j) const;
    int get(const float x, const float y) const;
};

#endif // MAP_H