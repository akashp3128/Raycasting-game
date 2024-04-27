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


int Map::get(const size_t i, const size_t j) {
    assert(i < w && j < h && sizeof(mapData) == w * h + 1);
    char c = mapData[i + j * w];
    if (std::isdigit(c)) {
        return c - '0';
    } else {
        // Return a default or invalid texture ID value
        // or handle the non-digit character case as per your requirements
        return 0;
    }
}

bool Map::is_empty(const size_t i, const size_t j) {
    assert(i < w && j < h && sizeof(mapData) == w * h + 1);
    return mapData[i + j * w] == ' ';
}