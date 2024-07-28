#ifndef MAP_H
#define MAP_H

#include <cstddef>

class Map {
public:
    static const size_t w = 16;
    static const size_t h = 16;
    static const char mapData[];

    Map();
    int get(const size_t i, const size_t j) const;
    bool is_empty(const size_t i, const size_t j) const;
    size_t width() const { return w; }
    size_t height() const { return h; }
    
    // Add this line to declare the print function
    void print() const;
};

#endif // MAP_H