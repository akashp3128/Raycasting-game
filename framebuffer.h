#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <cstdint>

class FrameBuffer {
public:
    FrameBuffer(const size_t width, const size_t height);
    void set_pixel(const size_t x, const size_t y, const uint32_t color);
    void draw_rectangle(const size_t rect_x, const size_t rect_y, const size_t rect_w, const size_t rect_h, const uint32_t color);
    void clear(const uint32_t color);

    size_t w, h;
    std::vector<uint32_t> img;
};

#endif // FRAMEBUFFER_H