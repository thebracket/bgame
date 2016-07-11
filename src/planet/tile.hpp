#pragma once

/*
New tile structure:

base_type - one of empty, solid, floor, wall, stairs (up/down/both), ramp, fortification, etc.
tile_material - stone type, air, soil, sand, etc.
tile_vegetation - for floors only
tile_special
hit_points - to allow for destruction!
flags - on fire, frozen, etc.
building_id
tree_id
navigation assistance
outside
separate out opaque from solid to allow for windows!

constexpr uint8_t base_type::empty = 0;
constexpr uint8_t base_type::solid = 1;
constexpr uint8_t base_type::floor = 2;
constexpr uint8_t base_type::wall = 3;
constexpr uint8_t base_type::ramp_up = 4;
constexpr uint8_t base_type::ramp_down = 5;
constexpr uint8_t base_type::stairs_down = 6;
constexpr uint8_t base_type::stairs_up = 7;
constexpr uint8_t base_type::stairs_updown = 8;

struct vegetation_t {
    uint16_t vegetation_type;
    uint8_t hit_points;
    uint8_t wetness_counter;
    uint16_t growth_counter;
};

struct tile_t {
	uint8_t base_type;
    uint8_t hit_points;
    // flags for special type/status
    // flags for navigation assistance
    std::size_t building_id;
    std::size_t tree_id;    
};

// A vector of rltk::vchar render_as

*/