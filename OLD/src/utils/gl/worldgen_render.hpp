#pragma once

#include <vector>

struct worldgen_tile {
    float x;
    float y;
    float alt1;
    float alt2;
    float alt3;
    float alt4;
    float red, green, blue;
    uint16_t glyph;
};

struct worldgen_unit {
    float x;
    float y;
    float altitude;
    uint16_t glyph;
};

struct worldgen_scene {
    int layer_texture;
    void add_tile(const worldgen_tile &tile);
    void add_unit(const worldgen_unit &unit);

    void render();

    std::vector<worldgen_tile> tiles;
    std::vector<worldgen_unit> units;
};
