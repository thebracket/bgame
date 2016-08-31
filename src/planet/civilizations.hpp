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
    int max_size = 0;

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

constexpr uint8_t MAX_GOV_TYPE = 13;
constexpr uint8_t GOV_ANARCHY = 1;
constexpr uint8_t GOV_FEUDAL = 2;
constexpr uint8_t GOV_DICTATORSHIP = 3;
constexpr uint8_t GOV_COMMUNIST = 4;
constexpr uint8_t GOV_TECHNOCRACY = 5;
constexpr uint8_t GOV_DEMOCRACY = 6;
constexpr uint8_t GOV_REPUBLIC = 7;
constexpr uint8_t GOV_OLIGARCHY = 8;
constexpr uint8_t GOV_FASCIST = 9;
constexpr uint8_t GOV_CONSTITUTIONAL_MONARCHY = 10;
constexpr uint8_t GOV_WARLORD = 11;
constexpr uint8_t GOV_TOTALITARIAN = 12;
constexpr uint8_t GOV_THEOCRACY = 13;
const std::vector<std::string> GOVERNMENT_NAMES{"", "Anarchy", "Feudal", "Dictatorship", "Communist", "Technocracy",
    "Democracy", "Republic", "Oligarchy", "Fascist", "Constitutional Monarchy", "Warlord", "Totalitarian",
    "Theocracy"};

struct civ_t {
    std::string name;
    std::string species_tag;
    std::unordered_map<std::size_t, int> relations;
    bool extinct = false;
    uint8_t tech_level = 1;
    uint8_t r,g,b;
    uint8_t gov_type;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);    
};

constexpr uint8_t MAX_OCCUPATION = 15;
constexpr uint8_t OCC_LABORER = 1;
constexpr uint8_t OCC_FARMER = 2;
constexpr uint8_t OCC_HERBALIST = 3;
constexpr uint8_t OCC_METALWORKER = 4;
constexpr uint8_t OCC_STONEWORKER = 5;
constexpr uint8_t OCC_WOODWORKER = 6;
constexpr uint8_t OCC_COOK = 7;
constexpr uint8_t OCC_HUNTER = 8;
constexpr uint8_t OCC_SKIRMISHER = 9;
constexpr uint8_t OCC_LIGHT_INFANTRY = 10;
constexpr uint8_t OCC_INFANTRY = 11;
constexpr uint8_t OCC_HEAVY_INFANTRY = 12;
constexpr uint8_t OCC_LIGHT_CAVALRY = 13;
constexpr uint8_t OCC_MEDIUM_CAVALRY = 14;
constexpr uint8_t OCC_HEAVY_CAVALRY = 15;
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
    uint8_t occupation = 0;
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