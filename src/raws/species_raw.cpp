#include "species.hpp"
#include "lua_bridge.hpp"
#include "apihelper.hpp"
#include "defs/civilization_t.hpp"
#include "graphviz.hpp"
#include <boost/container/flat_map.hpp>
#include "../utils/system_log.hpp"

boost::container::flat_map<std::string, raw_species_t> species_defs;
boost::container::flat_map<std::string, civilization_t> civ_defs;

const raw_species_t * get_species_def(const std::string &tag) noexcept
{
    auto finder = species_defs.find(tag);
    if (finder == species_defs.end()) return nullptr;
    return &finder->second;
}

civilization_t * get_civ_def(const std::string &tag) noexcept {
    auto finder = civ_defs.find(tag);
    if (finder == civ_defs.end()) return nullptr;
    return &finder->second;
}

void each_civilization_def(const std::function<void(std::string, civilization_t *)> &func) noexcept {
	for (auto &it : civ_defs) {
        func(it.first, &it.second);
    }
}

std::size_t get_species_defs_size() noexcept
{
    return species_defs.size();
}

std::string get_species_nth_tag(const int &roll) noexcept {
    auto it = species_defs.begin();
    for (auto i=0; i<roll; ++i) ++it;
    return it->first;
}

void sanity_check_species() noexcept
{
	for (auto &s : species_defs) {
        if (s.first.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species with no tag.");
        if (s.second.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no name: {0}", s.second.tag);
        if (s.second.male_name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no male name: {0}",s.second.tag);
        if (s.second.female_name.empty())glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no female name: {0}", s.second.tag);
        if (s.second.collective_name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no collective name: {0}", s.second.tag);
        if (s.second.stat_mods.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species has no stat modifiers: {0}", s.second.tag);
        if (s.second.body_parts.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Species with no body parts: {0}", s.second.tag);
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
            if (field == "tech_level") civ.tech_level = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
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
                        if (ufield == "bp_per_turn") unit.bp_per_turn = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
                        if (ufield == "speed") unit.speed = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
                        if (ufield == "name") unit.name = lua_tostring(lua_state, -1);
                        if (ufield == "worldgen_strength") unit.worldgen_strength = static_cast<int>(lua_tonumber(lua_state, -1));

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
                                    if (sfield == "n") sentient.n_present = static_cast<int>(lua_tonumber(lua_state, -1));
                                    if (sfield == "name") sentient.name = lua_tostring(lua_state, -1);
                                    if (sfield == "level") sentient.base_level = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
                                    if (sfield == "armor_class") sentient.base_armor_class = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
                                    if (sfield == "hp_n") sentient.hp_n = static_cast<int>(lua_tonumber(lua_state, -1));
                                    if (sfield == "hp_dice") sentient.hp_dice = static_cast<int>(lua_tonumber(lua_state, -1));
                                    if (sfield == "hp_mod") sentient.hp_mod = static_cast<int>(lua_tonumber(lua_state, -1));
                                    if (sfield == "gender") sentient.gender = lua_tostring(lua_state, -1);
                                    if (sfield == "natural_attacks") {
                                        civ_unit_natural_attack_t nattack;
                                        lua_pushstring(lua_state, sfield.c_str());
                                        lua_gettable(lua_state, -2);
                                        while (lua_next(lua_state, -2) != 0) {
                                            const std::string afield = lua_tostring(lua_state, -2);
                                            if (afield == "type") nattack.type = lua_tostring(lua_state, -1);
                                            if (afield == "hit_bonus") nattack.hit_bonus = static_cast<int>(lua_tonumber(lua_state, -1));
                                            if (afield == "n_dice") nattack.n_dice = static_cast<int>(lua_tonumber(lua_state, -1));
                                            if (afield == "die_type") nattack.die_type = static_cast<int>(lua_tonumber(lua_state, -1));
                                            if (afield == "die_mod") nattack.die_mod = static_cast<int>(lua_tonumber(lua_state, -1));
                                            if (afield == "range") nattack.range = static_cast<int>(lua_tonumber(lua_state, -1));

                                            lua_pop(lua_state, 1);
                                        }
                                        sentient.natural_attacks.push_back(nattack);
                                    }
                                    if (sfield == "equipment") {
                                        civ_equipment_t equip;
                                        lua_pushstring(lua_state, sfield.c_str());
                                        lua_gettable(lua_state, -2);
                                        while (lua_next(lua_state, -2) != 0) {
                                            std::string afield = lua_tostring(lua_state, -2);
                                            if (afield == "melee") equip.melee = lua_tostring(lua_state, -1);
                                            if (afield == "ranged") equip.ranged = lua_tostring(lua_state, -1);
                                            if (afield == "ammo") equip.ammo = lua_tostring(lua_state, -1);
                                            if (afield == "mount") equip.mount = lua_tostring(lua_state, -1);
                                            if (afield == "both" || afield == "male" || afield == "female") {
                                                lua_pushstring(lua_state, afield.c_str());
                                                lua_gettable(lua_state, -2);
                                                while (lua_next(lua_state, -2) != 0) {
                                                    std::string slot = lua_tostring(lua_state, -2);
                                                    std::string item = lua_tostring(lua_state, -1);
                                                    int gender_tag = 0;
                                                    if (afield == "male") gender_tag = 1;
                                                    if (afield == "female") gender_tag = 2;
                                                    equip.starting_clothes.emplace_back(std::make_tuple( gender_tag, slot, item ));

                                                    lua_pop(lua_state, 1);
                                                }
                                            }

                                            lua_pop(lua_state, 1);
                                        }
                                        sentient.equipment = equip;
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
                    const std::string target = lua_tostring(lua_state, -1);
                    civ.evolves_into.push_back(target);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "can_build") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string target = lua_tostring(lua_state, -1);
                    civ.can_build.push_back(target);
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }

        civ_defs[key] = civ;
        //std::cout << "Loaded civ: " << key << "\n";

        lua_pop(lua_state, 1);
    }
}

void read_species_types() noexcept
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
                    int value = static_cast<int>(lua_tonumber(lua_state, -1));
                    s.stat_mods.insert(std::make_pair(subfield, value));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "ethics") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string subfield = lua_tostring(lua_state, -2);
                    if (subfield == "diet") {
                        const std::string diet_type = lua_tostring(lua_state, -1);
                        if (diet_type == "omnivore") s.diet = diet_omnivore;
                        if (diet_type == "herbivore") s.diet = diet_herbivore;
                        if (diet_type == "carnivore") s.diet = diet_carnivore;
                    }
                    if (subfield == "alignment") {
                        const std::string alignment_type = lua_tostring(lua_state, -1);
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
                        const std::string part_field = lua_tostring(lua_state, -2);
                        if (part_field == "qty") std::get<1>(part) = static_cast<int>(lua_tonumber(lua_state, -1));
                        if (part_field == "size") std::get<2>(part) = static_cast<int>(lua_tonumber(lua_state, -1));
                        lua_pop(lua_state, 1);
                    }
                    s.body_parts.push_back(part);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "max_age") s.max_age = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "infant_age") s.infant_age = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "child_age") s.child_age = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "glyph") s.glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
            if (field == "glyph_ascii") s.glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
            if (field == "worldgen_glyph") s.worldgen_glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
            if (field == "composite_render") s.render_composite = lua_toboolean(lua_state, -1);
			if (field == "vox") s.voxel_model = lua_tonumber(lua_state, -1);
            if (field == "base_male_glyph") s.base_male_glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
            if (field == "base_female_glyph") s.base_female_glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
            if (field == "skin_colors") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string title = lua_tostring(lua_state, -2);
                    auto col = read_lua_color(title);
                    s.skin_colors.emplace_back(std::make_pair(title, col));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "hair_colors") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string title = lua_tostring(lua_state, -2);
                    bengine::color_t col = read_lua_color(title);
                    s.hair_colors.emplace_back(std::make_pair(title, col));
                    lua_pop(lua_state, 1);
                }
            }


            lua_pop(lua_state, 1);
        }
        species_defs[key] = s;

        lua_pop(lua_state, 1);
    }

    read_civ_types();
}

void make_civ_tree(graphviz_t * tree) {
	for (const auto &it : civ_defs) {
        const auto species = species_defs.find(it.second.species_tag);
        const auto species_name = species->second.tag;

        // Evolutionary options
        for (const auto &evolve : it.second.evolves_into) {
            tree->add_node(species_name, evolve);
        }

        // Units
        for (const auto &unit : it.second.units) {
            tree->add_node(species_name, unit.second.tag, graphviz_t::graphviz_shape_t::PARALLELOGRAM);
        }

        for (const auto &build : it.second.can_build) {
            tree->add_node(species_name, build, graphviz_t::graphviz_shape_t::HOUSE);
        }
    }
}