#include "map.h"

 const char Map::map[] = "1010222122220000"
                         "1              0"
                         "1     11111    0"
                         "1        3     0"
                         "4    2    3  110"
                         "4    5    3    0"
                         "0    14000     0"
                         "3    0    100  0"
                         "3    0    1 0  0"
                         "3    1  5   2  2"
                         "0    1      2  2"
                         "2    1    1    0"
                         "0         0    0"
                         "0    0000000   0"
                         "0              0"
                         "0002244222200000";

Map::Map() {
    assert(sizeof(map) == w * h + 1); // +1 for the null-terminated string
}

int Map::get(const size_t i, const size_t j) {
    assert(i < w && j < h && sizeof(map) == w * h + 1);
    return map[i + j * w] - '0';
}

bool Map::is_empty(const size_t i, const size_t j) {
    assert(i < w && j < h && sizeof(map) == w * h + 1);
    return map[i + j * w] == ' ';
}