#include "creatures.hpp"
#include "lua_bridge.hpp"
#include "apihelper.hpp"
#include "defs/raw_creature_t.hpp"
#include "../utils/system_log.hpp"

std::unordered_map<std::string, raw_creature_t> creature_defs;

const raw_creature_t * get_creature_def(const std::string &tag) noexcept
{
    return api_search<raw_creature_t>(creature_defs, tag);
}

void sanity_check_creatures() noexcept
{
    for (auto s = creature_defs.begin(); s!=creature_defs.end(); ++s) {
        if (s->first.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species with no tag.");
        if (s->second.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no name: {0}", s->second.tag);
        if (s->second.male_name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no male name: {0}", s->second.tag);
        if (s->second.female_name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no female name: {0}", s->second.tag);
        if (s->second.collective_name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no collective name: {0}", s->second.tag);
        if (s->second.body_parts.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no body parts: {0}", s->second.tag);
        if (s->second.attacks.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no attacks: {0}", s->second.tag);
        for (const auto &a : s->second.attacks) {
            if (a.type.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species attack with no name: {0}", s->second.tag);
        }
    }
}

void read_creature_types() noexcept
{
    lua_getglobal(lua_state, "creatures");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        raw_creature_t s;
        std::string key = lua_tostring(lua_state, -2);
        s.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") s.name = lua_tostring(lua_state, -1);
            if (field == "male_name") s.male_name = lua_tostring(lua_state, -1);
            if (field == "female_name") s.female_name = lua_tostring(lua_state, -1);
            if (field == "group_name") s.collective_name = lua_tostring(lua_state, -1);
            if (field == "description") s.description = lua_tostring(lua_state, -1);
            if (field == "stats") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string subfield = lua_tostring(lua_state, -2);
                    auto value = static_cast<int>(lua_tonumber(lua_state, -1));
                    s.stats.insert(std::make_pair(subfield, value));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "parts") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string part_name = lua_tostring(lua_state, -2);
                    std::tuple<std::string, int, int> part;
                    std::get<0>(part) = part_name;
                    lua_pushstring(lua_state, part_name.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string part_field = lua_tostring(lua_state, -2);
                        if (part_field == "qty") std::get<1>(part) = static_cast<int>(lua_tonumber(lua_state, -1));
                        if (part_field == "size") std::get<2>(part) = static_cast<int>(lua_tonumber(lua_state, -1));
                        lua_pop(lua_state, 1);
                    }
                    s.body_parts.push_back(part);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "combat") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string cname = lua_tostring(lua_state, -2);
                    if (cname == "armor_class") s.armor_class = static_cast<int>(lua_tonumber(lua_state, -1));
                    if (cname == "attacks") {
                        lua_pushstring(lua_state, cname.c_str());
                        lua_gettable(lua_state, -2);
                        while (lua_next(lua_state, -2) != 0) {
                            std::string attack_name = lua_tostring(lua_state, -2);
                            lua_pushstring(lua_state, attack_name.c_str());
                            lua_gettable(lua_state, -2);
                            creature_attack_t attack;
                            while (lua_next(lua_state, -2) != 0) {
                                const std::string attack_field = lua_tostring(lua_state, -2);
                                if (attack_field == "type") attack.type = lua_tostring(lua_state, -1);
                                if (attack_field == "hit_bonus") attack.hit_bonus = static_cast<int>(lua_tonumber(lua_state, -1));
                                if (attack_field == "n_dice") attack.damage_n_dice = static_cast<int>(lua_tonumber(lua_state, -1));
                                if (attack_field == "die_type") attack.damage_dice = static_cast<int>(lua_tonumber(lua_state, -1));
                                if (attack_field == "die_mod") attack.damage_mod = static_cast<int>(lua_tonumber(lua_state, -1));
                                lua_pop(lua_state, 1);
                            }
                            //std::cout << attack.type << attack_name << "\n";
                            s.attacks.push_back(attack);
                            lua_pop(lua_state, 1);
                        }
                    }

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "hunting_yield") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string yield_type = lua_tostring(lua_state, -2);
                    const auto value = static_cast<int>(lua_tonumber(lua_state, -1));
                    if (yield_type == "meat") s.yield_meat = value;
                    if (yield_type == "hide") s.yield_hide = value;
                    if (yield_type == "bone") s.yield_bone = value;
                    if (yield_type == "skull") s.yield_skull = value;
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "ai") {
                const std::string ai_type = lua_tostring(lua_state, -1);
                if (ai_type == "grazer") s.ai = creature_grazer;
            }
            if (field == "glyph") s.glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
            if (field == "glyph") s.glyph_ascii = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
			if (field == "vox") s.vox = static_cast<int>(lua_tonumber(lua_state, -1));
			if (field == "hp_n") s.hp_n = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "hp_dice") s.hp_dice = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "hp_mod") s.hp_mod = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "group_size_n_dice") s.group_size_n_dice = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "group_size_dice") s.group_size_dice = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "group_size_mod") s.group_size_mod = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "color") s.fg = read_lua_color("color");

            lua_pop(lua_state, 1);
        }
        creature_defs[key] = s;

        lua_pop(lua_state, 1);
    }
}
