#pragma once

#include <tuple>

extern bool world_changed;

struct map_render_t {
    int layer_texture;
    void render();
};

namespace map_render {
    std::tuple<int,int,int> readback_texture_pixel(const int &x, const int &y);
}