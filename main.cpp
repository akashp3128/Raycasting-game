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
#include "utility.h"
#include "framebuffer.h"
#include "map.h"
#include "player.h"

int wall_x_texcoord(const float hitx, const float hity, const Texture &tex_walls) {
    float x = hitx - floor(hitx + 1);
    float y = hity - floor(hity);
    int tex = static_cast<int>(x * (tex_walls.size - 1));
    if (std::abs(y) > std::abs(x))
        tex = static_cast<int>(y * (tex_walls.size - 1));
    if (tex < 0)
        tex += tex_walls.size - 1;
    assert(tex >= 0 && tex < static_cast<int>(tex_walls.size));
    return tex;
}

void render(FrameBuffer &fb, const Map &map, Texture &tex_walls, float player_x, float player_y, float player_a, float fov) {
    fb.clear(pack_color(200, 200, 200));

    const size_t rect_w = fb.w / (map.width() * 2);
    const size_t rect_h = fb.h / map.height();

    // Draw the map (top-down view)
    for (size_t j = 0; j < map.height(); j++) {
        for (size_t i = 0; i < map.width(); i++) {
            if (map.is_empty(i, j)) continue;
            size_t rect_x = i * rect_w;
            size_t rect_y = j * rect_h;
            size_t texid = map.get(i, j);
            if (texid >= tex_walls.count) {
                std::cerr << "Invalid texture ID in map: " << texid << " at (" << i << ", " << j << ")" << std::endl;
                continue;
            }
           // uint32_t color = pack_color(0, 0, texid); // Blue for walls...change this to add texture

            //fb.draw_rectangle(rect_x, rect_y, rect_w, rect_h, color);
            for (size_t ty = 0; ty < rect_h; ty++)
            {
                for (size_t tx = 0; tx < rect_w; tx++)
                {
                    size_t tex_x = (tx * tex_walls.size) / rect_w;
                    size_t tex_y = (ty * tex_walls.size) / rect_h;
                    uint32_t color = tex_walls.get(tex_x, texid, tex_y);
                    fb.set_pixel(rect_x + tx, rect_y + ty, color);
                }
            }
        }
    }

    // Cast rays and draw walls
    for (size_t i = 0; i < fb.w / 2; i++) {
        float angle = player_a - fov / 2 + fov * i / static_cast<float>(fb.w / 2);
        for (float t = 0; t < 20; t += 0.01) {
            float cx = player_x + t * cos(angle);
            float cy = player_y + t * sin(angle);

            size_t pix_x = static_cast<size_t>(cx * rect_w);
            size_t pix_y = static_cast<size_t>(cy * rect_h);
            
            fb.set_pixel(pix_x, pix_y, pack_color(255, 0, 0));

            if (!map.is_empty(cx, cy)) {
                size_t texid = map.get(cx, cy);
                if (texid >= tex_walls.count) {
                    std::cerr << "Invalid texture ID: " << texid << " at map position (" << cx << ", " << cy << ")" << std::endl;
                    continue;
                }

                float cos_angle = cos(angle - player_a);
                if (std::abs(cos_angle) < 1e-6) {
                    continue;
                }
                size_t column_height = std::min(static_cast<size_t>(fb.h / (t * cos_angle)), fb.h);

                int tex_coord = wall_x_texcoord(cx, cy, tex_walls);
                int pix_x = static_cast<int>(i + fb.w / 2);

                for (size_t j = 0; j < column_height; j++) {
                    int pix_y = static_cast<int>(j + fb.h / 2 - column_height / 2);
                    if (pix_x >= 0 && pix_x < static_cast<int>(fb.w) && pix_y >= 0 && pix_y < static_cast<int>(fb.h)) {
                        uint8_t intensity = static_cast<uint8_t>(255 * j / column_height);
                        fb.set_pixel(pix_x, pix_y, pack_color(0, intensity, 0));
                    }
                }
                break;
            }
        }
    }

    // Draw player position and direction
    size_t player_pix_x = static_cast<size_t>(player_x * rect_w);
    size_t player_pix_y = static_cast<size_t>(player_y * rect_h);
    fb.draw_rectangle(player_pix_x - 2, player_pix_y - 2, 5, 5, pack_color(255, 255, 0));
    
    size_t dir_x = player_pix_x + static_cast<size_t>(cos(player_a) * 20);
    size_t dir_y = player_pix_y + static_cast<size_t>(sin(player_a) * 20);
    fb.draw_line(player_pix_x, player_pix_y, dir_x, dir_y, pack_color(255, 0, 0));

    std::string angle_text = "Angle: " + std::to_string(player_a);
    fb.draw_text(10, 10, angle_text, pack_color(255, 0, 0));
}

int main() {
    const size_t win_w = 1024;
    const size_t win_h = 512;

    FrameBuffer fb(win_w, win_h);
    Map map;

    float player_x = 10.456;
    float player_y = 10.345;
    float player_a = 10.1245;
    const float fov = M_PI / 3.0;

    Texture tex_walls("walltext.png");
    if (!tex_walls.count) {
        std::cerr << "Failed to load wall textures!" << std::endl;
        return 1;
    }
    std::cout << "Successfully loaded " << tex_walls.count << " wall textures." << std::endl;

    for (size_t frame = 0; frame < 360; frame++) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player_a += 2 * M_PI / 360;

        std::cout << "Rendering frame " << frame << ", player angle: " << player_a << std::endl;

        render(fb, map, tex_walls, player_x, player_y, player_a, fov);
        drop_ppm_image(ss.str(), fb.img, fb.w, fb.h);

        std::ifstream file_check(("output/" + ss.str()).c_str());
        if (!file_check) {
            std::cerr << "Failed to create file: " << ss.str() << std::endl;
            break;
        }
        file_check.close();
    }

    return 0;
}