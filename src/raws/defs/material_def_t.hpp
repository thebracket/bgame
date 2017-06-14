#pragma once

#include <string>
#include <vector>

/*
 * Defines a material type. Used by just about everything.
 */
struct material_def_t {
    std::string tag = "";
    std::string name = "";
    material_def_spawn_type_t spawn_type = rock;
    std::string parent_material_tag = "";
    uint16_t glyph;
    rltk::color_t fg;
    rltk::color_t bg;
    uint8_t hit_points = 0;
    std::string mines_to_tag = "";
    std::string mines_to_tag_second = "";
    std::string layer = "";
    std::vector<std::string> ore_materials;
    int damage_bonus = 0;
    float ac_bonus = 0.0F;
    int floor_texture = 0;
    int constructed_floor_texture = 2;
    int wall_texture = 1;
    int constructed_wall_texture = 3;
};
