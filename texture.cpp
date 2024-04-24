#include "stb_image.h"
#include <vector>
#include <cstdint>
#include <iostream>

std::vector<uint8_t> loadTexture(const std::string& filename) {
    int width, height, channels;
    uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return {};
    }

    std::vector<uint8_t> texture(data, data + width * height * channels);
    stbi_image_free(data);

    return texture;
}