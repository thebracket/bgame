#include "plants.hpp"
#include "lua_bridge.hpp"
#include "defs/plant_t.hpp"
#include "../utils/system_log.hpp"

std::unordered_map<std::string, std::size_t> plant_defs_idx;
std::vector<plant_t> plant_defs;

std::size_t get_plant_idx(const std::string &tag) noexcept
{
    const auto finder = plant_defs_idx.find(tag);
    if (finder != plant_defs_idx.end()) {
        return finder->second;
    } else {
		glog(log_target::GAME, log_severity::warning, "WARNING: Cannot find plant - {0}", tag);
    }
    return 0;
}

plant_t * get_plant_def(const std::size_t &index) noexcept
{
	if (index > plant_defs.size()) return nullptr;
    return &plant_defs[index];
}

void sanity_check_plants() noexcept
{
    for (const auto &p : plant_defs) {
        if (p.name.empty()) glog(log_target::GAME, log_severity::warning, "WARNING: No plant name");
    }
}

void read_plant_types() noexcept
{
    plant_t p;
    std::string tag;

    read_lua_table("vegetation",
                   [&p, &tag] (const auto &key) { tag=key; p=plant_t{}; p.tag = tag; },
                   [&p] (const auto &key) { plant_defs.push_back(p); },
                   lua_parser{
                           {"name",    [&p]() { p.name = lua_str(); }},
                           {"cycles",  [&p]() {
								p.lifecycle.resize(5);
								lua_pushstring(lua_state, "cycles");
								lua_gettable(lua_state, -2);
								while (lua_next(lua_state, -2) != 0)
								{
									const std::string n = lua_tostring(lua_state, -2);
									int days = 0;
									days = lua_tonumber(lua_state, -1);

									if (n == "A") p.lifecycle[0] = days;
									if (n == "B") p.lifecycle[1] = days;
									if (n == "C") p.lifecycle[2] = days;
									if (n == "D") p.lifecycle[3] = days;
									if (n == "E") p.lifecycle[4] = days;
									lua_pop(lua_state, 1);
								}
                           }},
                           {"glyphs_ascii",  [&p]() {
                               lua_pushstring(lua_state, "glyphs_ascii");
                               lua_gettable(lua_state, -2);
							   p.glyphs_ascii.resize(4);
                               while(lua_next(lua_state, -2) != 0)
                               {
                                   const std::string n = lua_tostring(lua_state, -2);
                                   xp::vchar ap;
                                   lua_pushstring(lua_state, n.c_str());
                                   lua_gettable(lua_state, -2);
                                   while(lua_next(lua_state, -2) != 0)
                                   {
                                       const std::string field = lua_tostring(lua_state, -2);
                                       if (field == "glyph") ap.glyph = lua_tonumber(lua_state, -1);
                                       if (field == "col") ap.foreground = read_lua_color("col");
                                       lua_pop(lua_state, 1);
                                   }

								   if (n == "A") p.glyphs_ascii[0] = ap;
								   if (n == "B") p.glyphs_ascii[1] = ap;
								   if (n == "C") p.glyphs_ascii[2] = ap;
								   if (n == "D") p.glyphs_ascii[3] = ap;
								   lua_pop(lua_state, 1);
                               }
                           }},
                           {"harvest", [&p]() {
                               read_lua_table_inner("harvest", [&p](auto h) { p.provides.push_back(h); });
                           }},
                           {"tags",    [&p]() {
                               read_lua_table_inner("tags", [&p](auto t) {
                                   if (t == "spread") p.tags.set(PLANT_SPREADS);
                                   if (t == "annual") p.tags.set(PLANT_ANNUAL);
                               });
                           }}
                   }
    );

    /*lua_getglobal(lua_state, "vegetation");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        plant_t p;
        p.tag = key;
        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while(lua_next(lua_state, -2) != 0)
        {
            std::string field = lua_tostring(lua_state, -2);
            if (field == "name") p.name = lua_tostring(lua_state, -1);
            //if (field == "glyph") p.glyph = lua_tonumber(lua_state, -1);
            //if (field == "fg") p.fg = read_lua_color("fg");
            //if (field == "bg") p.bg = read_lua_color("bg");
            //if (field == "provides") p.provides = lua_tostring(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        plant_defs.push_back(p);
        if (p.provides.size() > 1) {
            //tech_tree_file << key << " -> farming -> item_" << p.provides << "\n";
        }
        lua_pop(lua_state, 1);
    }*/

    std::sort(plant_defs.begin(), plant_defs.end(), [] (plant_t a, plant_t b) { return a.tag < b.tag; });
    for (std::size_t i=0; i<plant_defs.size(); ++i) {
        plant_defs_idx[plant_defs[i].tag] = i;
    }
}
