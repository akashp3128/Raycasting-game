#ifndef MAP_H
#define MAP_H

#include <cstdlib>
#include <cassert>

class Map {
public:
    Map();
    int get(const size_t i, const size_t j);
    bool is_empty(const size_t i, const size_t j);
    size_t width() const { return w; }
    size_t height() const { return h; }

private:
    static const size_t w = 16;
    static const size_t h = 16;
    static const char map[];
};

#endif // MAP_H