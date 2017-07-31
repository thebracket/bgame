#include "clothing.hpp"
#include "apihelper.hpp"
#include "lua_bridge.hpp"
#include "defs/clothing_t.hpp"
#include <iostream>

std::unordered_map<std::string, clothing_t> clothing_types;

const clothing_t * get_clothing_by_tag(const std::string &tag) noexcept
{
    return api_search<clothing_t>(clothing_types, tag);
}

void sanity_check_clothing() noexcept
{
    for (auto it = clothing_types.begin(); it != clothing_types.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Empty clothing string\n";
        if (it->second.name.empty()) std::cout << "WARNING: Empty clothing name\n";
        if (it->second.colors.empty()) std::cout << "WARNING: " << it->first << " contains no color options\n";
        if (it->second.slot.empty()) std::cout << "WARNING: " << it->first << " has no slot defined.\n";
    }
}

void read_clothing() noexcept
{
    std::string tag = "";
    clothing_t c;
    read_lua_table("clothing",
                   [&c, &tag] (const auto &key) { tag = key; c = clothing_t{}; },
                   [&c, &tag] (const auto &key) {
                       clothing_types[key] = c;
                   },
                   lua_parser{
                           {"name",        [&c] () { c.name = lua_str(); }},
                           {"slot",        [&c] () { c.slot = lua_str(); }},
                           {"description", [&c] () { c.description = lua_str(); }},
                           {"ac",          [&c] () { c.armor_class = lua_float(); }},
                           {"glyph",       [&c] () { c.clothing_glyph = lua_int(); }},
                           {"colors",      [&c] () {
                               read_lua_table_inner( "colors", [&c] (auto col) { c.colors.push_back(col); });
                           }}
                   }
    );
}
