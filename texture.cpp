#include "texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &filename) : img_w(0), img_h(0), count(0), size(0) {
    int w, h, ch;
    unsigned char *pixmap = stbi_load(filename.c_str(), &w, &h, &ch, 0);
    if (!pixmap) {
        std::cerr << "Error: can't load the texture" << std::endl;
        return;
    }

    img_w = w;
    img_h = h;
    count = 6; // We have 6 textures in a row
    size = w / count;

    img = std::vector<uint32_t>(w*h);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            uint8_t r = pixmap[(i+j*w)*ch+0];
            uint8_t g = pixmap[(i+j*w)*ch+1];
            uint8_t b = pixmap[(i+j*w)*ch+2];
            uint8_t a = ch==4 ? pixmap[(i+j*w)*ch+3] : 255;
            img[i+j*w] = (a<<24) + (b<<16) + (g<<8) + r;
        }
    }
    stbi_image_free(pixmap);
}

uint32_t Texture::get(const size_t i, const size_t j) const {
    return img[i + j*img_w];
}

std::vector<uint32_t> Texture::get_scaled_column(size_t texture_x, size_t texture_id, size_t column_height) const {
    std::vector<uint32_t> column(column_height);
    for (size_t y = 0; y < column_height; y++) {
        size_t src_x = texture_x + texture_id*size;
        size_t src_y = (y*size)/column_height;
        column[y] = get(src_x, src_y);
    }
    return column;
}