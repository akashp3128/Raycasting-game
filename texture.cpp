#define STB_IMAGE_IMPLEMENTATION
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
        // Improved error reporting with stbi_failure_reason
        std::cerr << "Failed to load texture: " << filename << " - " << stbi_failure_reason() << std::endl;
        return;
    }

    // Detailed logging of loaded texture properties
    std::cout << "Loaded texture: " << filename << " (Width: " << width << ", Height: " << height 
              << ", Channels: " << channels << ")" << std::endl;

    if (4 != channels) {
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

    std::cout << "Number of Textures: " << count << ", Texture Size: " << size << std::endl;

    img.resize(width * height);

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

uint32_t& Texture::get(const size_t tex_coord, const size_t texture_id, const size_t j) {
    assert(tex_coord < size && j < size && texture_id < count);
    size_t index = tex_coord + texture_id * size * size + j * size;
    assert(index < img.size());
    return img[index];
}

std::vector<uint32_t> Texture::get_scaled_column(const size_t tex_coord, const size_t texture_id, const size_t column_height) {
    assert(tex_coord < size && texture_id < count);

    if (column_height > std::numeric_limits<size_t>::max() / sizeof(uint32_t)) {
        std::cerr << "Error: column_height exceeds maximum allowed size." << std::endl;
        return std::vector<uint32_t>();
    }

    std::vector<uint32_t> column(column_height);
    for (size_t y = 0; y < column_height; y++) {
        size_t j = static_cast<size_t>(y * static_cast<float>(size) / column_height);
        column[y] = get(tex_coord, texture_id, j);
    }

    return column;
}