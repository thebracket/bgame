#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <rltk.hpp>
#include <array>
#include "constants.hpp"

struct civ_t {
    std::string species_tag = "";
    uint8_t tech_level = 0;
    bool extinct = false;
    std::string name = "";
    std::string leader_name = "";
    std::string origin = "";
    uint8_t r=0;
    uint8_t g=0;
    uint8_t b=0;
    uint16_t glyph = 0;
    int startx = 0;
    int starty = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(species_tag, tech_level, extinct, name, leader_name, r, g, b, glyph, origin);
    }
};

struct region_info_t {
    std::size_t owner_civ = 0;
    uint8_t blight_level = 0;
    uint8_t settlement_size = 0;
    std::vector<std::string> improvements;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(owner_civ, blight_level, improvements);
    }
};

struct unit_t {
    std::size_t owner_civ = 0;
    uint16_t world_x = 0;
    uint16_t world_y = 0;
    std::string unit_type = "";
    bool dead=false;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(owner_civ, world_x, world_y, unit_type);
    }
};

struct civ_holder_t {
    std::vector<civ_t> civs;
    std::vector<unit_t> units;
    std::vector<region_info_t> region_info;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civs, units, region_info );
    }
};
