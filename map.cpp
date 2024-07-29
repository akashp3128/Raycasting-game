#include "map.h"

const char Map::map[] = "0000222222220000"\
                        "1              0"\
                        "1     111111   0"\
                        "1     0        0"\
                        "0     0  1110000"\
                        "0     3        0"\
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

int Map::get(const size_t i, const size_t j) const {
    if (i<w && j<h) return map[i+j*w] - '0';
    return -1;
}

int Map::get(const float x, const float y) const {
    return get(static_cast<size_t>(x), static_cast<size_t>(y));
}