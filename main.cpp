#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <sstream>
#include <iomanip>

#include "texture.h"
#include "utility.h"
#include "framebuffer.h"
#include "map.h"
#include "player.h"

// Function to calculate the texture coordinate for a given wall intersection point
int wall_x_texcoord(const float hitx, const float hity, Texture &tex_walls) {
    // Calculate the hit position relative to the cell
    float x = hitx - floor(hitx+1);
    float y = hity - floor(hity);

    // Calculate the texture coordinate based on the hit position
    int tex = static_cast<int>(x * tex_walls.size);



    if (std::abs(y)>std::abs(x)) // we need to determine whether we hit a "vertical" or a "horizontal" wall (w.r.t the map)
        tex = y*tex_walls.size;
    if (tex<0) // do not forget x_texcoord can be negative, fix that
        tex += tex_walls.size;
    assert(tex>=0 && tex < static_cast <int>(tex_walls.size));
    return tex;


}

// Function to render the scene
void render(FrameBuffer &fb, Map &map, Texture &tex_walls, float player_x, float player_y, float player_a, float fov) {
    // Clear the framebuffer
    fb.clear(pack_color(255, 255, 255));

    // Calculate the size of each cell on the screen
    const size_t rect_w = fb.w / (map.width() * 2);
    const size_t rect_h = fb.h / map.height();

    // Render the map
    for (size_t j = 0; j < map.height(); j++) {
        for (size_t i = 0; i < map.width(); i++) {
            if (map.is_empty(i, j)) continue;

            size_t rect_x = i * rect_w;
            size_t rect_y = j * rect_h;
            size_t texid = map.get(i, j);
            assert(texid < tex_walls.count);

            uint32_t color = tex_walls.get(0, 0, texid);
            fb.draw_rectangle(rect_x, rect_y, rect_w, rect_h, color);
        }
    }

    // Render the visibility cone
    for (size_t i = 0; i < fb.w / 2; i++) {
        float angle = player_a - fov / 2 + fov * i / static_cast<float>(fb.w / 2);

        for (float t = 0; t < 20; t += 0.01) {
            float cx = player_x + t * cos(angle);
            float cy = player_y + t * sin(angle);


            size_t pix_x = cx * rect_w;
            size_t pix_y = cy * rect_h;
            fb.set_pixel(pix_x, pix_y, pack_color(160, 160, 160));

            // Check if the cell is a wall
            // Check if the cell is a wall
            if (!map.is_empty(cx, cy)) {
                size_t texid = map.get(cx, cy);

            if (texid >= tex_walls.count) {
                // Skip if the texture ID is out of range
                continue;
             }

            // Calculate the height of the wall column
            float cos_angle = cos(angle - player_a);
            if (std::abs(cos_angle) < 1e-6) {
                // Skip if cos_angle is very small or close to zero
                continue;
             }
            size_t column_height = std::min(static_cast<size_t>(fb.h / (t * cos_angle)), fb.h);

            // Calculate the texture coordinate for the wall
            int tex_coord = wall_x_texcoord(cx, cy, tex_walls);

            int pix_x = i + fb.w / 2;

            std::vector<uint32_t> column = tex_walls.get_scaled_column(tex_coord, texid, column_height);

            // Draw the wall column
            for (size_t j = 0; j < column_height; j++) {
                int pix_y = j + fb.h / 2 - column_height / 2;
                if (pix_x >= 0 && pix_x < static_cast<int>(fb.w) && pix_y >= 0 && pix_y < static_cast<int>(fb.h)) {
                    fb.set_pixel(pix_x, pix_y, column[j]);
                }
            }

            break;
            }
        }
    }
}


int main() {
    // Set up the window dimensions
    const size_t win_w = 1024;
    const size_t win_h = 512;

    // Create the framebuffer
    FrameBuffer fb(win_w, win_h);

    // Create the map
    Map map;

    // Set up the player's initial position and angle
    float player_x = 3.456;
    float player_y = 2.345;
    float player_a = 8.1245;
    const float fov = M_PI / 3.0;

    // Load the wall textures
    Texture tex_walls("walltext.png");


    // Render and save frames
    for (size_t frame = 0; frame < 360; frame++) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player_a += 2 * M_PI / 360;

        render(fb, map, tex_walls, player_x, player_y, player_a, fov);
        drop_ppm_image(ss.str(), fb.img, fb.w, fb.h);
    }

    return 0;
}