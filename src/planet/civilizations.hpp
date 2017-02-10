#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <rltk.hpp>

struct settlement_t {
    std::size_t civ_id;
    std::string name;
    int world_x, world_y;
    uint8_t status = 1;
    bool deleted = false;
    uint8_t abandoned_years = 0;
    int max_size = 0;

    uint8_t blight_level = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civ_id, name, world_x, world_y, status, deleted, abandoned_years, max_size, blight_level );
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
    std::string species_tag;
    std::unordered_map<std::size_t, int> relations;
    bool extinct = false;
    uint8_t tech_level = 1;
    uint8_t r,g,b;
    int cordex_feelings = 0;
    bool met_cordex = false;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( name, species_tag, relations, extinct, tech_level, r, g, b, cordex_feelings, met_cordex );
    }
};

struct civ_person_t {
    std::size_t civ_id;
    std::string species_tag;
    std::string caste;
    int world_x, world_y;
    bool male;
    bool married;
    std::size_t married_to;
    int age;
    bool deceased;
    std::size_t mother_id = 0;
    std::size_t father_id = 0;
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
        archive( civ_id, species_tag, world_x, world_y, male, married, married_to, age, deceased, mother_id, father_id, level, caste );
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
};