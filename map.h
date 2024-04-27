#ifndef MAP_H
#define MAP_H

#include <cstddef>

struct Map {
    Map();

    size_t w = 16;
    size_t h = 16;

    static const char mapData[];

    int get(const size_t i, const size_t j);
    bool is_empty(const size_t i, const size_t j);

    size_t width() const { return w; }
    size_t height() const { return h; }
};

#endif