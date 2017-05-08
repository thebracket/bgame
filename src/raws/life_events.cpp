#include "life_events.hpp"
#include "lua_bridge.hpp"

std::unordered_map<std::string, life_event_template> life_event_defs;

void read_life_events() noexcept
{
    std::string tag = "";
    life_event_template le;
    read_lua_table("life_events",
                   [&le, &tag] (const auto &key) { tag = key; le = life_event_template{}; },
                   [&le, &tag] (const auto &key) { life_event_defs[tag] = le; },
                   lua_parser{
                           {"min_age", [&le] ()        { le.min_age = lua_int(); }},
                           {"max_age", [&le] ()        { le.max_age = lua_int(); }},
                           {"weight", [&le] ()         { le.weight = lua_int(); }},
                           {"description", [&le] ()    { le.description = lua_str(); }},
                           {"modifiers", [&le] ()     {
                               read_stat_modifiers("modifiers", le);
                           }},
                           {"skills",      [&le] () {
                               read_lua_table_inner( "skills", [&le] (auto s) { le.skills.push_back(s); });
                           }},
                           {"requires",      [&le] () {
                               read_lua_table_inner( "requires", [&le] (auto s) { le.requires_event.push_back(s); });
                           }},
                           {"precludes",      [&le] () {
                               read_lua_table_inner( "precludes", [&le] (auto s) { le.precludes_event.push_back(s); });
                           }}
                   }
    );
}