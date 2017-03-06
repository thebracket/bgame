#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <rltk.hpp>
#include <array>
#include "constants.hpp"

struct civ_t {
    std::size_t species_idx = 0;
    uint8_t tech_level = 0;
    bool extinct = false;
    std::string name = "";

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(species_idx, tech_level, extinct, name);
    }
};

struct region_info_t {
    std::size_t owner_civ = 0;
    uint8_t blight_level = 0;
    uint8_t settlement_size = 0;
    std::vector<uint16_t> improvements;

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
    uint8_t unit_type = 0;

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

/*
struct settlement_t {
    std::size_t civ_id;
    std::string name;
    int world_x, world_y;
    uint8_t status = 1;
    bool deleted = false;
    uint8_t abandoned_years = 0;
    int max_size = 0;

    uint8_t blight_level = 0;
    std::vector<std::string> improvements;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civ_id, name, world_x, world_y, status, deleted, abandoned_years, max_size, blight_level, improvements );
    }
};

inline void civ_cull_settlements(std::vector<settlement_t> &settlements) {
    for (auto &s : settlements) {
        if (s.status == 0) ++s.abandoned_years;
        if (s.abandoned_years > 20) s.deleted = true;
    }

    settlements.erase(std::remove_if(settlements.begin(), settlements.end(), [] (settlement_t &s) { return s.deleted; }), settlements.end());
}

struct civ_t {
    std::string name;
    std::size_t species;
    std::unordered_map<std::size_t, int> relations;
    bool extinct = false;
    uint8_t tech_level = 1;
    uint8_t r,g,b;
    int cordex_feelings = 0;
    bool met_cordex = false;
    int research_level = 0;
    uint16_t glyph;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( name, species, relations, extinct, tech_level, r, g, b, cordex_feelings, met_cordex, research_level, glyph );
    }
};

struct civ_person_t {
    std::size_t civ_id = 0;
    std::size_t species = 0;
    std::size_t caste = 0;
    std::string behavior = "";
    int world_x, world_y;
    bool male = true;
    int age = 10;
    bool deceased = false;
    int level = 1;
    int str = 10;
    int dex = 10;
    int con = 10;
    int intelligence = 10;
    int wis = 10;
    int cha = 10;
    int com = 10;
    int ethics = 10;
    int hit_points = 10;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civ_id, species, world_x, world_y, male, age, deceased, level, caste, behavior );
    }
};

struct civ_holder_t {
    std::vector<civ_t> civs;
    std::vector<civ_person_t> population;
    std::vector<settlement_t> settlements;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civs, population, settlements );
    }
};*/
