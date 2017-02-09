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

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civ_id, name, world_x, world_y, status, deleted, abandoned_years, max_size );
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

struct unimportant_person_t {
    std::size_t civ_id;
    std::string species_tag;
    int world_x, world_y;
    bool male;
    bool married;
    std::size_t married_to;
    int age;
    bool deceased;
    std::size_t mother_id = 0;
    std::size_t father_id = 0;
    int level = 1;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civ_id, species_tag, world_x, world_y, male, married, married_to, age, deceased, mother_id, father_id, level );
    }
};

struct civ_holder_t {
    std::vector<civ_t> civs;
    std::vector<unimportant_person_t> unimportant_people;
    std::vector<settlement_t> settlements;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( civs, unimportant_people, settlements );
    }
};