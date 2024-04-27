#include "map.h"
#include <cassert>
#include <iostream>
#include <string>

const char Map::mapData[] = "1010222122220000"
                            "1000000000000000"
                            "1000001111100000"
                            "1000030000000000"
                            "4000203011000000"
                            "4000503000000000"
                            "0001400000000000"
                            "3000000100000000"
                            "3000001000000000"
                            "3000105002000000"
                            "0000001000200000"
                            "2000000100100000"
                            "0000000000000000"
                            "0000000000000000"
                            "0000000000000000"
                            "0002244222200000";

Map::Map() {
    assert(sizeof(mapData) == w * h + 1); // +1 for the null-terminated string
}

int Map::get(const size_t i, const size_t j) {
    assert(i < w && j < h && sizeof(mapData) == w * h + 1);
    return mapData[i + j * w] - '0';
}

bool Map::is_empty(const size_t i, const size_t j) {
    assert(i < w && j < h && sizeof(mapData) == w * h + 1);
    return mapData[i + j * w] == ' ';
}