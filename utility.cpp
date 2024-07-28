#include "utility.h"
#include <cassert>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    return (a << 24) + (b << 16) + (g << 8) + r;
}

void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a)
{
    r = (color >> 0) & 255;
    g = (color >> 8) & 255;
    b = (color >> 16) & 255;
    a = (color >> 24) & 255;
}

void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t w, const size_t h) {
    assert(image.size() == w * h);

    std::string directory = "output/";
    std::string filepath = directory + filename;

    // Create the output directory if it doesn't exist
    mkdir(directory.c_str(), 0755);

    std::ofstream ofs(filepath, std::ios::binary | std::ios::trunc);
    ofs << "P6\n" << w << " " << h << "\n255\n";

    for (size_t i = 0; i < h * w; ++i) {
        uint8_t r, g, b, a;
        unpack_color(image[i], r, g, b, a);
        ofs.put(r);
        ofs.put(g);
        ofs.put(b);
    }

    ofs.close();
}