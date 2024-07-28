#include "map.h"
#include <cassert>
#include <iostream>
#include <string>

const char Map::mapData[]=  "0011223344220000"\
                            "5              0"\
                            "3   5  1 11111 0"\
                            "1      3       0"\
                            "0   1  3   10000"\
                            "0   1  3       0"\
                            "0   10000      0"\
                            "0   0   11100  0"\
                            "0   0   0      0"\
                            "0   0   1  00000"\
                            "0       1      0"\
                            "2       1      0"\
                            "0       0      0"\
                            "0 0000000      0"\
                            "0              0"\
                            "0002222222200000"; 

Map::Map() {
    assert(sizeof(mapData) == w * h + 1); // +1 for the null-terminated string
}

int Map::get(const size_t i, const size_t j) const {
    assert(i < w && j < h && sizeof(mapData) == w * h + 1);
    char c = mapData[i + j * w];
    if (c >= '0' && c <= '5') {  // Assuming texture IDs are 0-5
        return c - '0';
    } else if (c == ' ') {
        return -1;  // Empty space
    } else {
        std::cerr << "Warning: Invalid map data at (" << i << ", " << j << "): '" << c << "'" << std::endl;
        return 0;  // Default to first texture if invalid
    }
}

bool Map::is_empty(const size_t i, const size_t j) const {
    assert(i < w && j < h && sizeof(mapData) == w * h + 1);
    return mapData[i + j * w] == ' ';
}

// Debug function to print the map
void Map::print() const {
    for (size_t j = 0; j < h; j++) {
        for (size_t i = 0; i < w; i++) {
            std::cout << mapData[i + j * w];
        }
        std::cout << std::endl;
    }
}