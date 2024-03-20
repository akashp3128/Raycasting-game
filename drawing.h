#ifndef DRAWING_H
#define DRAWING_H

#include <cstddef>
#include <cstdint>
#include <vector>

void draw_rectangle(std::vector<uint32_t> &img, const size_t img_w, const size_t img_h, const size_t x, const size_t y, const size_t w, const size_t h, const uint32_t color);

#endif