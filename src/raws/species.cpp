#include "species.hpp"
#include "lua_bridge.hpp"
#include "apihelper.hpp"

std::unordered_map<std::string, raw_species_t> species_defs;
std::unordered_map<std::string, raw_civilized_t> civ_defs;

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

void read_civ_species(std::ofstream &tech_tree_file) noexcept
{
    lua_getglobal(lua_state, "civilized_species");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        raw_civilized_t s;
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
            if (field == "baby_name") s.baby_name = lua_tostring(lua_state, -1);
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
                    if (subfield == "mentality") s.ethics.mentality = lua_tostring(lua_state, -1);
                    if (subfield == "blight") s.ethics.spread_blight = lua_toboolean(lua_state, -1);
                    if (subfield == "behavior") s.ethics.behavior = lua_tostring(lua_state, -1);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "max_age") s.max_age = lua_tonumber(lua_state, -1);
            if (field == "infant_age") s.infant_age = lua_tonumber(lua_state, -1);
            if (field == "child_age") s.child_age = lua_tonumber(lua_state, -1);
            if (field == "glyph") s.glyph = lua_tonumber(lua_state, -1);
            if (field == "breed_type") s.breed_type = lua_tostring(lua_state, -1);
            if (field == "clutch_size_min") s.clutch_size_min = lua_tonumber(lua_state, -1);
            if (field == "clutch_size_max") s.clutch_size_max = lua_tonumber(lua_state, -1);
            if (field == "clutch_frequency") s.clutch_frequency = lua_tonumber(lua_state, -1);
            if (field == "gains_tech_by_eating") s.gains_tech_by_eating = lua_toboolean(lua_state, -1);
            if (field == "lisp") s.lisp = lua_toboolean(lua_state, -1);
            if (field == "never_negotiates") s.never_negotiates = lua_toboolean(lua_state, -1);
            if (field == "castes") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    caste_t caste;
                    std::string caste_type = lua_tostring(lua_state, -2);
                    caste.tag = caste_type;
                    lua_pushstring(lua_state, caste_type.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string subfield = lua_tostring(lua_state, -2);
                        if (subfield == "fertile") caste.fertile = lua_toboolean(lua_state, -1);
                        if (subfield == "hp_n") caste.hp_n = lua_tonumber(lua_state, -1);
                        if (subfield == "hp_dice") caste.hp_n = lua_tonumber(lua_state, -1);
                        if (subfield == "hp_mod") caste.hp_n = lua_tonumber(lua_state, -1);
                        if (subfield == "spreads_blight") caste.spreads_blight = lua_toboolean(lua_state, -1);
                        if (subfield == "destroys_everything") caste.destroys_everything = lua_toboolean(lua_state, -1);
                        if (subfield == "berserk") caste.berserk = lua_toboolean(lua_state, -1);
                        if (subfield == "max_per_region") caste.max_per_region = lua_tonumber(lua_state, -1);
                        if (subfield == "min_per_occupied_region") caste.min_per_occupied_region = lua_tonumber(lua_state, -1);
                        if (subfield == "starting_level") caste.starting_level = lua_tonumber(lua_state, -1);
                        if (subfield == "name") caste.name_override = lua_tostring(lua_state, -1);
                        if (subfield == "combat") {
                            lua_pushstring(lua_state, subfield.c_str());
                            lua_gettable(lua_state, -2);
                            while (lua_next(lua_state, -2) != 0) {
                                std::string cfield = lua_tostring(lua_state, -2);
                                if (cfield == "armor_class") caste.combat.base_armor_class = lua_tonumber(lua_state, -1);
                                if (cfield == "attacks") {
                                    lua_pushstring(lua_state, subfield.c_str());
                                    lua_gettable(lua_state, -2);
                                    while (lua_next(lua_state, -2) != 0) {
                                        std::string afield = lua_tostring(lua_state, -2);
                                        natural_attack_t attack;
                                        lua_pushstring(lua_state, afield.c_str());
                                        lua_gettable(lua_state, -2);
                                        while (lua_next(lua_state, -2) != 0) {
                                            std::string nafield = lua_tostring(lua_state, -2);
                                            if (nafield == "type") attack.type = lua_tostring(lua_state, -1);
                                            if (nafield == "hit_bonus") attack.hit_bonus = lua_tonumber(lua_state, -1);
                                            if (nafield == "n_dice") attack.n_dice = lua_tonumber(lua_state, -1);
                                            if (nafield == "die_type") attack.die_type = lua_tonumber(lua_state, -1);
                                            if (nafield == "die_mod") attack.die_mod = lua_tonumber(lua_state, -1);

                                            lua_pop(lua_state, 1);
                                        }
                                        caste.combat.natural_attacks.push_back(attack);
                                        lua_pop(lua_state, 1);
                                    }
                                }
                                lua_pop(lua_state, 1);
                            }
                        }
                        lua_pop(lua_state, 1);
                    }

                    s.castes.insert(std::make_pair(caste_type, caste));
                    lua_pop(lua_state, 1);
                }
                //lua_pop(lua_state, 1);
            }

            lua_pop(lua_state, 1);
        }

        civ_defs.insert(std::make_pair( s.tag, s ));
        lua_pop(lua_state, 1);
    }
    lua_pop(lua_state, 1);
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
                    }
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

    read_civ_species(tech_tree_file);
}
