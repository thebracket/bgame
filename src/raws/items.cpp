#include "items.hpp"
#include "lua_bridge.hpp"
#include "defs/item_def_t.hpp"
#include <boost/container/flat_map.hpp>
#include "../utils/system_log.hpp"

boost::container::flat_map<std::string, item_def_t> item_defs;
boost::container::flat_map<int, stockpile_def_t> stockpile_defs;
int clothing_stockpile = 0;

int get_clothing_stockpile() noexcept {
    return clothing_stockpile;
}

void set_clothing_stockpile(const int n) noexcept {
    clothing_stockpile = n;
}

item_def_t * get_item_def(const std::string &tag) noexcept {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) return nullptr;
    return &finder->second;
}

stockpile_def_t * get_stockpile_def(const int tag) noexcept {
    auto finder = stockpile_defs.find(tag);
    if (finder == stockpile_defs.end()) return nullptr;
    return &finder->second;
}

void each_stockpile(const std::function<void(stockpile_def_t *)> &func) noexcept {
	for (auto &it : stockpile_defs) {
        func(&it.second);
    }
}

void sanity_check_items() noexcept
{
	for (auto &it : item_defs) {
        if (it.first.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Item has no name");
        if (it.second.tag.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty item tag");
        if (it.second.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty item name, tag: {0}", it.second.tag);
    }
}

void read_items() noexcept
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
                           { "glyph_ascii", [&c] ()    { c.glyph_ascii = lua_int(); }},
                           { "damage_n", [&c] ()       { c.damage_n = lua_int(); }},
                           { "damage_d", [&c] ()       { c.damage_d = lua_int(); }},
                           { "damage_mod", [&c] ()     { c.damage_mod = lua_int(); }},
                           { "damage_stat", [&c] ()    { c.damage_stat = lua_str(); }},
                           { "ammo", [&c] ()           { c.ammo = lua_str(); }},
                           { "range", [&c] ()          { c.range = lua_int(); }},
						   { "vox", [&c]()             { c.voxel_model = lua_int(); }},
		                   { "clothing_layer", [&c]()  { c.clothing_layer = lua_int(); } },
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
                                   if (type == "drink") c.categories.set(ITEM_DRINK);
                                   if (type == "hide") c.categories.set(ITEM_HIDE);
                                   if (type == "bone") c.categories.set(ITEM_BONE);
                                   if (type == "skull") c.categories.set(ITEM_SKULL);
                                   if (type == "leather") c.categories.set(ITEM_LEATHER);
								   if (type == "tool-farming") c.categories.set(ITEM_FARMING);
								   if (type == "seed") c.categories.set(ITEM_SEED);
								   if (type == "topsoil") c.categories.set(ITEM_TOPSOIL);
								   if (type == "fertilizer") c.categories.set(ITEM_FERTILIZER);
								   if (type == "food-prepared") c.categories.set(ITEM_FOOD_PREPARED);
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
                   [&tag, &c] (const auto &key) { tag=key; c=stockpile_def_t{}; c.tag = key; },
                   [&c] (const auto &key) { stockpile_defs[c.index] = c; if (c.tag == "clothing") clothing_stockpile = c.index; },
                   lua_parser{
                           { "name", [&c] ()         { c.name = lua_str(); }},
                           { "id", [&c] ()           { c.index = lua_int(); }}
                   }
    );
}

void sanity_check_stockpiles() noexcept
{

}