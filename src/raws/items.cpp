#include "items.hpp"
#include "lua_bridge.hpp"

boost::container::flat_map<std::string, item_def_t> item_defs;
boost::container::flat_map<int, stockpile_def_t> stockpile_defs;
int clothing_stockpile = 0;

void sanity_check_items() noexcept
{
    for (auto it=item_defs.begin(); it!=item_defs.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Item has no name\n";
        if (it->second.tag.empty()) std::cout << "WARNING: Empty item tag\n";
        if (it->second.name.empty()) std::cout << "WARNING: Empty item name, tag: " << it->second.tag << "\n";
    }
}

void read_items(std::ofstream &tech_tree_file) noexcept
{

    std::string tag;
    item_def_t c;

    read_lua_table("items",
                   [&c, &tag] (const auto &key) { tag=key; c=item_def_t{}; c.tag = tag; },
                   [&c, &tag] (const auto &key) { item_defs[tag] = c; },
                   lua_parser{
                           { "name", [&c] ()           { c.name = lua_str(); }},
                           { "description", [&c] ()    { c.description = lua_str(); }},
                           { "background", [&c] ()     { c.bg = read_lua_color("background"); }},
                           { "foreground", [&c] ()     { c.bg = read_lua_color("foreground"); }},
                           { "glyph", [&c] ()          { c.glyph = lua_int(); }},
                           { "damage_n", [&c] ()       { c.damage_n = lua_int(); }},
                           { "damage_d", [&c] ()       { c.damage_d = lua_int(); }},
                           { "damage_mod", [&c] ()     { c.damage_mod = lua_int(); }},
                           { "damage_stat", [&c] ()    { c.damage_stat = lua_str(); }},
                           { "ammo", [&c] ()           { c.ammo = lua_str(); }},
                           { "range", [&c] ()          { c.range = lua_int(); }},
                           { "stack_size", [&c] ()     { c.stack_size = lua_int(); }},
                           { "initiative_penalty", [&c] ()    { c.initiative_penalty = lua_int(); }},
                           { "stockpile", [&c] ()      { c.stockpile_idx = lua_int(); }},
                           { "itemtype", [&c] () {
                               read_lua_table_inner( "itemtype", [&c] (auto type) {
                                   if (type == "component") c.categories.set(COMPONENT);
                                   if (type == "tool-chopping") c.categories.set(TOOL_CHOPPING);
                                   if (type == "tool-digging") c.categories.set(TOOL_DIGGING);
                                   if (type == "weapon-melee") c.categories.set(WEAPON_MELEE);
                                   if (type == "weapon-ranged") c.categories.set(WEAPON_RANGED);
                                   if (type == "ammo") c.categories.set(WEAPON_AMMO);
                                   if (type == "food") c.categories.set(ITEM_FOOD);
                                   if (type == "spice") c.categories.set(ITEM_SPICE);
                               });
                           }}
                   }
    );
}

void read_stockpiles() noexcept
{
    std::string tag;
    std::string name;
    stockpile_def_t c;

    read_lua_table("stockpiles",
                   [&tag, &name, &c] (const auto &key) { tag=key; c=stockpile_def_t{}; c.tag = key; },
                   [&tag, &name, &c] (const auto &key) { stockpile_defs[c.index] = c; if (c.tag == "clothing") clothing_stockpile = c.index; },
                   lua_parser{
                           { "name", [&c] ()         { c.name = lua_str(); }},
                           { "id", [&c] ()           { c.index = lua_int(); }}
                   }
    );
}

void sanity_check_stockpiles() noexcept
{

}