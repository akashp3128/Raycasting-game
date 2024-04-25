#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cassert>

#include "texture.h"
#include "utility.h"
#include "framebuffer.h"
#include "map.h"

int wall_x_texcoord(const float x, const float y, Texture &tex_walls) {
    float hitx = x - floor(x + 0.5);
    float hity = y - floor(y + 0.5);
    int tex = hitx * tex_walls.size;

    if (std::abs(hity) > std::abs(hitx))
        tex = hity * tex_walls.size;
    if (tex < 0)
        tex += tex_walls.size;
    assert(tex >= 0 && tex < (int)tex_walls.size);
    return tex;
}

int main() {
    const size_t win_w = 1024;
    const size_t win_h = 512;

    FrameBuffer fb(win_w, win_h);
    Map map;

    float player_x = 3.456;
    float player_y = 2.345;
    float player_a = 8.1245;
    const float fov = M_PI / 3.;

    const size_t rect_w = win_w / (map.width() * 2);
    const size_t rect_h = win_h / map.height();

    Texture wallTextureAtlas("pics/walltext.png");
    size_t wallTextureSize = wallTextureAtlas.size;
    size_t wallTextureCnt = wallTextureAtlas.count;

    std::cout << "Texture Atlas Dimensions: " << wallTextureAtlas.img_w << "x" << wallTextureAtlas.img_h << std::endl;
    std::cout << "Number of Textures: " << wallTextureCnt << std::endl;
    std::cout << "Number of Channels: 4" << std::endl;
    std::cout << "First few pixels: " << static_cast<int>(wallTextureAtlas.img[0]) << ", "
                                      << static_cast<int>(wallTextureAtlas.img[1]) << ", "
                                      << static_cast<int>(wallTextureAtlas.img[2]) << ", ..." << std::endl;

    for (size_t frame = 0; frame < 360; frame++) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player_a += 2 * M_PI / 360;

        fb.clear(pack_color(255, 255, 255));

        for (size_t j = 0; j < map.height(); j++) { //draw the map
            for (size_t i = 0; i < map.width(); i++) {
                if (map.is_empty(i, j)) continue; //skip empty spaces
                size_t rect_x = i * rect_w;
                size_t rect_y = j * rect_h;
                size_t texid = map.get(i, j);
                assert(texid < wallTextureCnt);

                uint32_t color = wallTextureAtlas.get(0, 0, texid);
                fb.draw_rectangle(rect_x, rect_y, rect_w, rect_h, color);
            }
        }

        //heart of the 3d engine
        for (size_t i = 0; i < win_w / 2; i++) { //draw visibility cone and 3D view
            float angle = player_a - fov / 2 + fov * i / float(win_w / 2);

            for (float t = 0; t < 20; t += .01) {
                float cx = player_x + t * cos(angle);
                float cy = player_y + t * sin(angle);
                float dx = cos(angle);
                float dy = sin(angle);

                size_t pix_x = cx * rect_w;
                size_t pix_y = cy * rect_h;
                fb.set_pixel(pix_x, pix_y, pack_color(160, 160, 160)); // this draws the visibility cone

                if (!map.is_empty(cx, cy)) { // our ray touches a wall, so draw the vertical column to create an illusion of 3D
                    size_t texid = map.get(cx, cy);
                    assert(texid < wallTextureCnt);

                    size_t column_height = win_h / (t * cos(angle - player_a));

                    wall_x_texcoord(cx, cy, wallTextureAtlas);
                    int pix_x = i + fb.w/2;

                    std::vector<uint32_t> column = wallTextureAtlas.get_scaled_column(texid, wallTextureSize, column_height);
                    for (size_t j = 0; j < column_height; j++) {
                        int pix_y = j + fb.h/2 - column_height/2;
                        if (pix_y>=0 && pix_y<(int)fb.h) {
                            fb.set_pixel(pix_x, pix_y, column[j]);
                        }
                    }

                    break;
                }
            }
        }

        drop_ppm_image(ss.str(), fb.img, fb.w, fb.h);
    }

    return 0;
}