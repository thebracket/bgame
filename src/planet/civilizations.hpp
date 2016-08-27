#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>

struct settlement_t {
    std::size_t civ_id;
    std::string name;
    int world_x, world_y;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);
};

struct civ_t {
    std::string name;
    std::string species_tag;
    std::unordered_map<std::size_t, int> relations;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);    
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
    int occupation = 0;

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