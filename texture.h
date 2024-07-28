#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include <cstdint>

class Texture {
public:
    size_t img_w, img_h; // overall image dimensions
    size_t count, size; // number of textures and size of each texture
    std::vector<uint32_t> img; // the array of pixels

    Texture(const std::string &filename);
    uint32_t& get(const size_t tex_coord, const size_t texture_id, const size_t j);
    std::vector<uint32_t> get_scaled_column(const size_t tex_coord, const size_t texture_id, const size_t column_height);
};

#endif // TEXTURE_H