#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "texture.h"
#include "map.h"
#include "player.h"
#include "framebuffer.h"

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a=255) {
    return (a<<24) + (b<<16) + (g<<8) + r;
}

void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
    r = (color >>  0) & 255;
    g = (color >>  8) & 255;
    b = (color >> 16) & 255;
    a = (color >> 24) & 255;
}

void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t w, const size_t h) {
    std::ofstream ofs(filename, std::ofstream::binary);
    ofs << "P6\n" << w << " " << h << "\n255\n";
    for (size_t i = 0; i < w*h; ++i) {
        uint8_t r, g, b, a;
        unpack_color(image[i], r, g, b, a);
        ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
    }
    ofs.close();
}

int wall_x_texcoord(const float hitx, const float hity, const Texture &tex_walls) {
    float x = hitx - floor(hitx);
    float y = hity - floor(hity);
    int tex = x > y ? static_cast<int>(x * tex_walls.size) : static_cast<int>(y * tex_walls.size);
    return tex % tex_walls.size;
}

void render(FrameBuffer &fb, const Map &map, Texture &tex_walls, Player &player) {
    fb.clear(pack_color(255, 255, 255)); // Clear screen to white

    for (size_t i = 0; i < fb.w; i++) {
        float angle = player.a - player.fov/2 + player.fov*i/float(fb.w);
        for (float t = 0; t < 20; t += .01) {
            float x = player.x + t*cos(angle);
            float y = player.y + t*sin(angle);
            
            int tex_id = map.get(x, y);
            if (tex_id == -1) continue;

            size_t column_height = fb.h/t*cos(angle-player.a);
            
            int tex_coord = wall_x_texcoord(x, y, tex_walls);
            std::vector<uint32_t> column = tex_walls.get_scaled_column(tex_coord, tex_id, column_height);
            int pix_x = i;
            for (size_t j = 0; j < column_height; j++) {
                int pix_y = j + fb.h/2 - column_height/2;
                if (pix_y >= 0 && pix_y < (int)fb.h) {
                    fb.set_pixel(pix_x, pix_y, column[j]);
                }
            }
            break;
        }
    }
}

int main() {
    FrameBuffer fb(1024, 512);
    Player player(3.456, 2.345, 1.523, M_PI/3);
    Map map;
    Texture tex_walls("walltext.png");

    for (size_t frame = 0; frame < 360; frame++) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player.a += 2*M_PI/360;
        render(fb, map, tex_walls, player);
        drop_ppm_image(ss.str(), fb.img, fb.w, fb.h);
    }

    return 0;
}