#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>

struct settlement_t {
    std::size_t civ_id;
    std::string name;
    int world_x, world_y;
    uint8_t status = 1;
    bool deleted = false;
    uint8_t abandoned_years = 0;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);
};

inline void civ_cull_settlements(std::vector<settlement_t> &settlements) {
    for (auto &s : settlements) {
        if (s.status == 0) ++s.abandoned_years;
        if (s.abandoned_years > 20) s.deleted = true;
    }

    settlements.erase(std::remove_if(settlements.begin(), settlements.end(), [] (settlement_t &s) { return s.deleted; }), settlements.end());
}

constexpr int MAX_GOV_TYPE = 16;
constexpr int GOV_KRATEROCRACY = 1;
constexpr int GOV_PLUTOCRACY = 2;
constexpr int GOV_GENIOCRACY = 3;
constexpr int GOV_MERITOCRACY = 4;
constexpr int GOV_TECHNOCRACY = 5;
constexpr int GOV_TIMOCRACY = 6;
constexpr int GOV_AUTOCRACY = 7;
constexpr int GOV_OLIGARCHY = 8;
constexpr int GOV_MONARCHY = 9;
constexpr int GOV_CONSTITUTIONAL_MONARCHY = 10;
constexpr int GOV_AUTHORITARIAN = 11;
constexpr int GOV_TOTALITARIAN = 12;
constexpr int GOV_DEMOCRACY = 13;
constexpr int GOV_DEMARCHY = 14;
constexpr int GOV_REPUBLIC = 15;
constexpr int GOV_THEOCRACY = 16;
const std::vector<std::string> GOVERNMENT_NAMES{"", "Kraterocracy", "Plutocracy", "Geniocracy", "Meritocracy", "Technocracy",
    "Timocracy", "Autocracy", "Oligarchy", "Monarchy", "Constitutional Monarchy", "Authoritarian", "Totalitarian",
    "Democracy", "Demarchy", "Republic", "Theocracy"};

struct civ_t {
    std::string name;
    std::string species_tag;
    std::unordered_map<std::size_t, int> relations;
    bool extinct = false;
    uint8_t tech_level = 1;
    uint8_t r,g,b;
    int gov_type;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);    
};

constexpr int MAX_OCCUPATION = 15;
constexpr int OCC_LABORER = 1;
constexpr int OCC_FARMER = 2;
constexpr int OCC_HERBALIST = 3;
constexpr int OCC_METALWORKER = 4;
constexpr int OCC_STONEWORKER = 5;
constexpr int OCC_WOODWORKER = 6;
constexpr int OCC_COOK = 7;
constexpr int OCC_HUNTER = 8;
constexpr int OCC_SKIRMISHER = 9;
constexpr int OCC_LIGHT_INFANTRY = 10;
constexpr int OCC_INFANTRY = 11;
constexpr int OCC_HEAVY_INFANTRY = 12;
constexpr int OCC_LIGHT_CAVALRY = 13;
constexpr int OCC_MEDIUM_CAVALRY = 14;
constexpr int OCC_HEAVY_CAVALRY = 15;
const std::vector<std::string> OCCUPATION_NAMES{"Unemployed", "Laborer", "Farmer", "Herbalist", "Metalworker", 
    "Stoneworker", "Woodworker", "Cook", "Hunter", "Skirmisher", "Light Infantry", "Infantry", "Heavy Infantry",
    "Light Cavalry", "Medium Cavalry", "Heavy Cavalry"}; 

struct unimportant_person_t {
    std::size_t civ_id;
    std::string species_tag;
    int world_x, world_y;
    bool male;
    bool married;
    std::size_t married_to;
    int age;
    bool deceased;
    int occupation = 0;
    std::size_t mother_id = 0;
    std::size_t father_id = 0;
    int level = 1;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);
};

struct civ_holder_t {
    std::vector<civ_t> civs;
    std::vector<unimportant_person_t> unimportant_people;
    std::vector<settlement_t> settlements;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);
};