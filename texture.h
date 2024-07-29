#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include <cstdint>

class Texture {
public:
    size_t img_w, img_h; // texture dimensions
    size_t count;        // total number of textures
    size_t size;         // size of each texture (assuming square textures)
    std::vector<uint32_t> img; // the texture data

    Texture(const std::string &filename);
    uint32_t get(const size_t i, const size_t j) const;
    std::vector<uint32_t> get_scaled_column(size_t texture_x, size_t texture_id, size_t column_height) const;
};

#endif // TEXTURE_H