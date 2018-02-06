#pragma once

#include <vector>
#include <string>

/*
 * Defines a starting profession.
 */
struct profession_t {
    std::string name = "";
    short strength = 0;
    short dexterity = 0;
    short constitution = 0;
    short intelligence = 0;
    short wisdom = 0;
    short charisma = 0;
    short comeliness = 0;
    short ethics = 0;
    std::vector< std::tuple< uint8_t, std::string, std::string >> starting_clothes;
};

