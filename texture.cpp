
#include "stb_image.h"
#include "texture.h"
#include <vector>
#include <cstdint>
#include <iostream>
#include <cassert>
#include "utility.h"

Texture::Texture(const std::string& filename) : img_w(0), img_h(0), count(0), size(0), img() {
    int width, height, channels = -1;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return;
    }

    if (channels != 4) {
        std::cerr << "Error: the texture must be a 32-bit image" << std::endl;
        stbi_image_free(data);
        return;
    }

    if (width != height * int(width / height)) {
        std::cerr << "Error: the texture file must contain N square textures packed horizontally" << std::endl;
        stbi_image_free(data);
        return;
    }

    count = width / height;
    size = width / count;
    img_w = width;
    img_h = height;
    img = std::vector<uint32_t>(width * height);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            uint8_t r = data[(i + j * width) * 4 + 0];
            uint8_t g = data[(i + j * width) * 4 + 1];
            uint8_t b = data[(i + j * width) * 4 + 2];
            uint8_t a = data[(i + j * width) * 4 + 3];
            img[i + j * width] = pack_color(r, g, b, a);
        }
    }

    stbi_image_free(data);
}

uint32_t& Texture::get(const size_t i, const size_t j, const size_t idx) {
    assert(i < size && j < size && idx < count);
    return img[i + idx * size + j * img_w];
}

std::vector<uint32_t> Texture::get_scaled_column(const size_t texture_id, const size_t tex_coord, const size_t column_height) {
    assert(tex_coord < size && texture_id < count);
    std::vector<uint32_t> column(column_height);
    for (size_t y = 0; y < column_height; y++) {
        column[y] = get(tex_coord, (y * size) / column_height, texture_id);
    }
    return column;
}