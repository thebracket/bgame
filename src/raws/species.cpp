#include "species.hpp"
#include "lua_bridge.hpp"
#include "apihelper.hpp"

std::unordered_map<std::string, raw_species_t> species_defs;
std::unordered_map<std::string, civilization_t> civ_defs;

const raw_species_t * get_species_def(const std::string &tag) noexcept
{
    return api_search<raw_species_t>(species_defs, tag);
}

std::size_t get_species_defs_size() noexcept
{
    return species_defs.size();
}

std::string get_species_nth_tag(const int &roll) noexcept {
    auto it = species_defs.begin();
    for (int i=0; i<roll; ++i) ++it;
    return it->first;
}

void sanity_check_species() noexcept
{
    for (auto s = species_defs.begin(); s!=species_defs.end(); ++s) {
        if (s->first.empty()) std::cout << "WARNING: Species with no tag.\n";
        if (s->second.name.empty()) std::cout << "WARNING: Species has no name: " << s->second.tag << "\n";
        if (s->second.male_name.empty()) std::cout << "WARNING: Species has no male name: " << s->second.tag << "\n";
        if (s->second.female_name.empty()) std::cout << "WARNING: Species has no female name: " << s->second.tag << "\n";
        if (s->second.collective_name.empty()) std::cout << "WARNING: Species has no collective name: " << s->second.tag << "\n";
        if (s->second.stat_mods.empty()) std::cout << "WARNING: Species has no stat modifiers: " << s->second.tag << "\n";
        if (s->second.body_parts.empty()) std::cout << "WARNING: Species with no body parts: " << s->second.tag << "\n";
    }
}

void read_civ_types() noexcept
{
    lua_getglobal(lua_state, "civilizations");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);
        civilization_t civ;
        civ.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);
            if (field == "species_def") civ.species_tag = lua_tostring(lua_state, -1);
            if (field == "ai") civ.ai = lua_tostring(lua_state, -1);
            if (field == "name_generator") civ.name_generator = lua_tostring(lua_state, -1);
            if (field == "units") {
                // Read the units table

                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string unit_type = lua_tostring(lua_state, -2);
                    civ_unit_t unit;
                    unit.tag = unit_type;

                    lua_pushstring(lua_state, unit_type.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string ufield = lua_tostring(lua_state, -2);
                        if (ufield == "bp_per_turn") unit.bp_per_turn = lua_tonumber(lua_state, -1);
                        if (ufield == "speed") unit.speed = lua_tonumber(lua_state, -1);
                        if (ufield == "name") unit.name = lua_tostring(lua_state, -1);

                        if (ufield == "sentients") {
                            lua_pushstring(lua_state, ufield.c_str());
                            lua_gettable(lua_state, -2);
                            while (lua_next(lua_state, -2) != 0) {
                                std::string sentient_type = lua_tostring(lua_state, -2);
                                civ_unit_sentient_t sentient;
                                sentient.tag = sentient_type;

                                lua_pushstring(lua_state, sentient_type.c_str());
                                lua_gettable(lua_state, -2);
                                while (lua_next(lua_state, -2) != 0) {
                                    std::string sfield = lua_tostring(lua_state, -2);
                                    if (sfield == "n") sentient.n_present = lua_tonumber(lua_state, -1);
                                    if (sfield == "name") sentient.name = lua_tostring(lua_state, -1);
                                    if (sfield == "level") sentient.base_level = lua_tonumber(lua_state, -1);
                                    if (sfield == "armor_class") sentient.base_armor_class = lua_tonumber(lua_state, -1);
                                    if (sfield == "hp_n") sentient.hp_n = lua_tonumber(lua_state, -1);
                                    if (sfield == "hp_dice") sentient.hp_dice = lua_tonumber(lua_state, -1);
                                    if (sfield == "hp_mod") sentient.hp_mod = lua_tonumber(lua_state, -1);
                                    if (sfield == "gender") sentient.gender = lua_tostring(lua_state, -1);
                                    if (sfield == "natural_attacks") {
                                        civ_unit_natural_attack_t nattack;
                                        lua_pushstring(lua_state, sfield.c_str());
                                        lua_gettable(lua_state, -2);
                                        while (lua_next(lua_state, -2) != 0) {
                                            std::string afield = lua_tostring(lua_state, -2);
                                            if (afield == "type") nattack.type = lua_tostring(lua_state, -1);
                                            if (afield == "hit_bonus") nattack.hit_bonus = lua_tonumber(lua_state, -1);
                                            if (afield == "n_dice") nattack.n_dice = lua_tonumber(lua_state, -1);
                                            if (afield == "die_type") nattack.die_type = lua_tonumber(lua_state, -1);
                                            if (afield == "die_mod") nattack.die_mod = lua_tonumber(lua_state, -1);

                                            lua_pop(lua_state, 1);
                                        }
                                        sentient.natural_attacks.push_back(nattack);
                                    }

                                    lua_pop(lua_state, 1);
                                }
                                unit.sentients.push_back(sentient);
                                lua_pop(lua_state, 1);
                            }
                        }

                        lua_pop(lua_state, 1);
                    }

                    civ.units[unit_type] = unit;
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "evolves_into") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string target = lua_tostring(lua_state, -2);
                    civ.evolves_into.push_back(target);
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }

        civ_defs[key] = civ;

        lua_pop(lua_state, 1);
    }
}

void read_species_types(std::ofstream &tech_tree_file) noexcept
{
    lua_getglobal(lua_state, "species_sentient");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        raw_species_t s;
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
            if (field == "stat_mods") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string subfield = lua_tostring(lua_state, -2);
                    int value = lua_tonumber(lua_state, -1);
                    s.stat_mods.insert(std::make_pair(subfield, value));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "ethics") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string subfield = lua_tostring(lua_state, -2);
                    if (subfield == "diet") {
                        std::string diet_type = lua_tostring(lua_state, -1);
                        if (diet_type == "omnivore") s.diet = diet_omnivore;
                        if (diet_type == "herbivore") s.diet = diet_herbivore;
                        if (diet_type == "carnivore") s.diet = diet_carnivore;
                    }
                    if (subfield == "alignment") {
                        std::string alignment_type = lua_tostring(lua_state, -1);
                        if (alignment_type == "good") s.alignment = align_good;
                        if (alignment_type == "neutral") s.alignment = align_neutral;
                        if (alignment_type == "evil") s.alignment = align_evil;
                        if (alignment_type == "devour") s.alignment = align_devour;
                    }
                    if (subfield == "blight") s.spreads_blight = lua_toboolean(lua_state, -1);
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
                        std::string part_field = lua_tostring(lua_state, -2);
                        if (part_field == "qty") std::get<1>(part) = lua_tonumber(lua_state, -1);
                        if (part_field == "size") std::get<2>(part) = lua_tonumber(lua_state, -1);
                        lua_pop(lua_state, 1);
                    }
                    s.body_parts.push_back(part);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "max_age") s.max_age = lua_tonumber(lua_state, -1);
            if (field == "infant_age") s.infant_age = lua_tonumber(lua_state, -1);
            if (field == "child_age") s.child_age = lua_tonumber(lua_state, -1);
            if (field == "glyph") s.glyph = lua_tonumber(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        species_defs[key] = s;

        lua_pop(lua_state, 1);
    }

    read_civ_types();
}
