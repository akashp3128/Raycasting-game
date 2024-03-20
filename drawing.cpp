#include "drawing.h"
#include <cassert>


void draw_rectangle(std::vector<uint32_t> &img, const size_t img_w, const size_t img_h, const size_t x, const size_t y, const size_t w, const size_t h, const uint32_t color)
{
    assert(img.size() == img_w * img_h);
    for (size_t i = 0; i < w; i++) {
        for (size_t j = 0; j < h; j++) {
            size_t cx = x + i;
            size_t cy = y + j;
            if (cx >= img_w || cy >= img_h) continue;
            img[cx + cy * img_w] = color;
        }
    }
}