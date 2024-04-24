#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cassert>

#include "texture.h"
#include "stb_image.h"
#include "utility.h"
#include "drawing.h"
#include "game.h"

int main()
{
    const size_t win_w = 1024; // image width
    const size_t win_h = 512;  // image height
    std::vector<uint32_t> framebuffer(win_w * win_h, pack_color(255, 255, 255)); // the image itself, initialized to white

    assert(sizeof(map) == map_w * map_h + 1); // +1 for the null terminated string
    float player_x = 3.456; // player x position
    float player_y = 2.345; // player y position
    float player_a = 8.1245; // player view direction
    const float fov = M_PI / 3.; // field of view

    std::vector<uint32_t> colors(ncolors);
    for (size_t i = 0; i < ncolors; i++) {
        colors[i] = pack_color(rand() % 255, rand() % 255, rand() % 255);
        // colors[0] = pack_color(255, 0, 0);    // Red
        // colors[1] = pack_color(0, 255, 0);    // Green
        // colors[2] = pack_color(0, 0, 255);    // Blue
        // colors[3] = pack_color(255, 255, 0);  // Yellow
    }

    const size_t rect_w = win_w / (map_w * 2);
    const size_t rect_h = win_h / map_h;

    // Load texture data for walls
    std::vector<uint8_t> wallTextureAtlas = loadTexture("pics/walltext.png");
    int wallTextureWidth, wallTextureHeight, wallTextureChannels;
    stbi_info("pics/walltext.png", &wallTextureWidth, &wallTextureHeight, &wallTextureChannels);
    size_t wallTextureSize = wallTextureWidth;
    size_t wallTextureCnt = wallTextureWidth / wallTextureHeight;

    std::cout << "Texture Atlas Dimensions: " << wallTextureWidth << "x" << wallTextureHeight << std::endl;
    std::cout << "Number of Textures: " << wallTextureCnt << std::endl;
    std::cout << "Number of Channels: " << wallTextureChannels << std::endl;
    std::cout << "First few pixels: " << static_cast<int>(wallTextureAtlas[0]) << ", "
                                    << static_cast<int>(wallTextureAtlas[1]) << ", "
                                    << static_cast<int>(wallTextureAtlas[2]) << ", ..." << std::endl;

    for (size_t frame = 0; frame < 360; frame++) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player_a += 2 * M_PI / 360;

        framebuffer = std::vector<uint32_t>(win_w * win_h, pack_color(255, 255, 255));
        for (size_t j = 0; j < map_h; j++) { //draw the map
            for (size_t i = 0; i < map_w; i++) {
                if (map[i + j * map_w] == ' ') continue; //skip empty spaces
                size_t rect_x = i * rect_w;
                size_t rect_y = j * rect_h;
                size_t texid = map[i + j * map_w] - '0';
                assert(texid < wallTextureCnt);

                // Sample the color from the upper-left pixel of the texture
                size_t texture_offset = texid * wallTextureSize * wallTextureChannels;
                uint32_t color = pack_color(
                    wallTextureAtlas[texture_offset],
                    wallTextureAtlas[texture_offset + 1],
                    wallTextureAtlas[texture_offset + 2]
                );

        draw_rectangle(framebuffer, win_w, win_h, rect_x, rect_y, rect_w, rect_h, color);
    }
}

        //heart of the 3d engine
        for (size_t i = 0; i < win_w / 2; i++) { //draw visibility cone and 3D view
            float angle = player_a - fov / 2 + fov * i / float(win_w / 2);

            for (float t = 0; t < 20; t += .01) {
                float cx = player_x + t * cos(angle);
                float cy = player_y + t * sin(angle);

                size_t pix_x = cx * rect_w;
                size_t pix_y = cy * rect_h;
                framebuffer[pix_x + pix_y * win_w] = pack_color(160, 160, 160); // this draws the visibility cone

                if (map[int(cx) + int(cy) * map_w] != ' ') { // our ray touches a wall, so draw the vertical column to create an illusion of 3D
                    size_t icolor = map[int(cx) + int(cy) * map_w] - '0';
                    assert(icolor < ncolors);

                    // Calculate texture coordinates
                    float u = (float)(icolor % wallTextureCnt) / (float)wallTextureCnt;
                    float v = std::max(0.0f, std::min(1.0f, (float)(int(cy)) / (float)map_h));

                    size_t column_height = win_h / (t * cos(angle - player_a));

                    // Draw textured column
                    for (size_t y = 0; y < column_height; ++y) {
                        size_t texture_y = (v * wallTextureHeight) * wallTextureWidth * wallTextureChannels;
                        size_t texture_offset = texture_y + (u * wallTextureSize * wallTextureChannels);
                        framebuffer[win_w / 2 + i + (win_h / 2 - column_height / 2 + y) * win_w] = pack_color(
                            wallTextureAtlas[texture_offset + 0],
                            wallTextureAtlas[texture_offset + 1],
                            wallTextureAtlas[texture_offset + 2]
                        );
                    }

                    break;
                }
            }
        }

        drop_ppm_image(ss.str(), framebuffer, win_w, win_h);
    }

    return 0;
}