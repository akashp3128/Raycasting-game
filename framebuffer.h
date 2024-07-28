#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <cstdint>
#include <string>

class FrameBuffer {
private:
    static const unsigned char font8x8_basic[128][8];

public:
    size_t w, h;
    std::vector<uint32_t> img;

    FrameBuffer(const size_t width, const size_t height);
    void set_pixel(const size_t x, const size_t y, const uint32_t color);
    void draw_rectangle(const size_t rect_x, const size_t rect_y, const size_t rect_w, const size_t rect_h, const uint32_t color);
    void clear(const uint32_t color);
    void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
    void draw_text(int x, int y, const std::string& text, uint32_t color);
    void draw_char(int x, int y, char c, uint32_t color);
};

#endif // FRAMEBUFFER_H