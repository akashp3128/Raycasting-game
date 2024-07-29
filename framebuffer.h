#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <cstdint>

struct FrameBuffer {
    size_t w, h;
    std::vector<uint32_t> img;

    FrameBuffer(const size_t w, const size_t h) : w(w), h(h), img(w*h, 0) {}

    void set_pixel(const size_t x, const size_t y, const uint32_t color) {
        if (x<w && y<h) img[x + y*w] = color;
    }

    void clear(const uint32_t color) {
        img = std::vector<uint32_t>(w*h, color);
    }
};

#endif // FRAMEBUFFER_H