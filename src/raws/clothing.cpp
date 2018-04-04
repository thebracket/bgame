#include "clothing.hpp"
#include "apihelper.hpp"
#include "lua_bridge.hpp"
#include "defs/clothing_t.hpp"
#include "../utils/system_log.hpp"

std::unordered_map<std::string, clothing_t> clothing_types;

const clothing_t * get_clothing_by_tag(const std::string &tag) noexcept
{
    return api_search<clothing_t>(clothing_types, tag);
}

void sanity_check_clothing() noexcept
{
	for (auto &it : clothing_types) {
        if (it.first.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty clothing string");
        if (it.second.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty clothing name");
        if (it.second.colors.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING {0} contains no color options.", it.first);
        if (it.second.slot.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING {0} has no slot defined.", it.first);
    }
}

void read_clothing() noexcept
{
    std::string tag;
    clothing_t c;
    read_lua_table("clothing",
                   [&c, &tag] (const auto &key) { tag = key; c = clothing_t{}; },
                   [&c] (const auto &key) {
                       clothing_types[key] = c;
                   },
                   lua_parser{
                           {"name",        [&c] () { c.name = lua_str(); }},
                           {"slot",        [&c] () { c.slot = lua_str(); }},
                           {"description", [&c] () { c.description = lua_str(); }},
                           {"ac",          [&c] () { c.armor_class = lua_float(); }},
                           {"glyph",       [&c] () { c.clothing_glyph = lua_int(); }},
						   {"layer",       [&c]()  { c.clothing_layer = lua_int(); }},
						   {"vox",         [&c]()  { c.voxel_model = lua_int(); } },
				           {"colors",      [&c] () {
                               read_lua_table_inner( "colors", [&c] (auto col) { c.colors.push_back(col); });
                           }}
                   }
    );
}
