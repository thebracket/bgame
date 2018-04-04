#include "reactions.hpp"
#include "lua_bridge.hpp"
#include "materials.hpp"
#include "buildings.hpp"
#include "items.hpp"
#include "clothing.hpp"
#include "defs/item_def_t.hpp"
#include "graphviz.hpp"
#include "defs/reaction_t.hpp"
#include <boost/container/flat_map.hpp>
#include "../utils/system_log.hpp"

boost::container::flat_map<std::size_t, reaction_t> reaction_defs;
boost::container::flat_map<std::size_t, std::vector<std::string>> reaction_building_defs;

reaction_t * get_reaction_def(const std::string &tag) noexcept {
	auto finder = reaction_defs.find(std::hash<std::string>{}(tag));
    if (finder == reaction_defs.end()) return nullptr;
    return &finder->second;
}

std::vector<std::string> get_reactions_for_building(const std::string &tag) noexcept {
    const auto result = reaction_building_defs.find(std::hash<std::string>{}(tag));
    if (result == reaction_building_defs.end()) return std::vector<std::string>();
    return result->second;
}

void each_reaction(const std::function<void(std::string, reaction_t *)> &func) noexcept {
	for (auto &it : reaction_defs) {
        func(it.second.tag, &it.second);
    }
}

void read_reactions() noexcept
{
    lua_getglobal(lua_state, "reactions");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        reaction_t c;
        c.specials.reset();

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;
		c.hashtag = std::hash<std::string>{}(c.tag);

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "workshop") c.workshop = lua_tostring(lua_state, -1);
            if (field == "skill") c.skill = lua_tostring(lua_state, -1);
            if (field == "difficulty") c.difficulty = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "inputs") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);

                    reaction_input_t input;
                    input.required_material = 0;
                    input.required_material_type = NO_SPAWN_TYPE;
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") input.tag = lua_tostring(lua_state, -1);
                        if (f == "qty") input.quantity = static_cast<int>(lua_tonumber(lua_state, -1));
                        if (f == "material") {
                            const std::string mat_name = lua_tostring(lua_state, -1);
                            const auto matfinder = get_material_by_tag(mat_name);
                            if (matfinder == 0) {
								glog(log_target::LOADER, log_severity::warning, "Reaction {0} references unkown material {1}", c.name, mat_name);
                            } else {
                                input.required_material = matfinder;
                            }
                        }
                        if (f == "mat_type") {
                            const std::string type_s = lua_tostring(lua_state, -1);
                            if (type_s == "cluster_rock") {
                                input.required_material_type = CLUSTER_ROCK;
                            } else if (type_s == "rock") {
                                input.required_material_type = ROCK;
                            } else if (type_s == "soil") {
                                input.required_material_type = SOIL;
                            } else if (type_s == "sand") {
                                input.required_material_type = SAND;
                            } else if (type_s == "metal") {
                                input.required_material_type = METAL;
                            } else if (type_s == "synthetic") {
                                input.required_material_type = SYNTHETIC;
                            } else if (type_s == "organic") {
                                input.required_material_type = ORGANIC;
                            } else if (type_s == "leather") {
                                input.required_material_type = LEATHER;
                            } else if (type_s == "food") {
                                input.required_material_type = FOOD;
                            } else if (type_s == "spice") {
                                input.required_material_type = SPICE;
                            } else {
								glog(log_target::LOADER, log_severity::warning, "WARNING: Unknown material type: {0}", type_s);
                            }
                        }
                        lua_pop(lua_state, 1);
                    }
                    c.inputs.push_back(input);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "outputs") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);
                    std::pair<std::string, int> comp;
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") comp.first = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.second = static_cast<int>(lua_tonumber(lua_state, -1));
                        if (f == "special") {
                            const std::string special = lua_tostring(lua_state, -1);
                            if (special == "cooking") c.specials.set(special_reaction_cooking);
                            if (special == "tanning") c.specials.set(special_reaction_tanning);
                        }
                        lua_pop(lua_state, 1);
                    }
                    c.outputs.push_back(comp);
                    //tech_tree_file << "\"" << key << "\"" << comp.first << "\"\n";

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "automatic") c.automatic = lua_toboolean(lua_state, -1);
            if (field == "power_drain") c.power_drain = static_cast<int>(lua_tonumber(lua_state, -1));

            lua_pop(lua_state, 1);
        }
        reaction_defs[c.hashtag] = c;
		reaction_building_defs[std::hash<std::string>{}(c.workshop)].push_back(key);

        lua_pop(lua_state, 1);
    }
}

void sanity_check_reactions() noexcept
{
	for (auto &it : reaction_defs) {
        if (it.second.tag.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty reaction tag");
        if (it.second.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty reaction name, tag: {0}", it.first);
        if (it.second.workshop.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty workshop name, tag: {0}", it.first);
        const auto bf = get_building_def(it.second.workshop);
        if (bf == nullptr) glog(log_target::LOADER, log_severity::warning, "WARNING: Undefined workshop, tag: {0}", it.first);
        for (const auto &input : it.second.inputs) {
            const auto finder = get_item_def(input.tag);
            if (finder == nullptr) glog(log_target::LOADER, log_severity::warning, "WARNING: Unknown item tag in input: {0}, reaction tag {1}", input.tag, it.first);
        }
        for (const auto &output : it.second.outputs) {
            const auto finder = get_item_def(output.first);
            const auto finder2 = get_clothing_by_tag(output.first);
            if (finder == nullptr && !finder2) glog(log_target::LOADER, log_severity::warning, "WARNING: Unknown item tag in output: {0}, reaction tag {1}", output.first, it.first);
        }
    }
}

void build_reaction_tree(graphviz_t * tree) {
	for (const auto &it : reaction_defs) {
        tree->add_node(std::string("building_") + it.second.workshop, it.second.tag, graphviz_t::graphviz_shape_t::HOUSE);
        for (const auto &input : it.second.inputs) {
            tree->add_node(std::string("item_") + input.tag, it.second.tag);
        }
        for (const auto &output : it.second.outputs) {
            tree->add_node(it.second.tag, std::string("item_") + output.first, graphviz_t::graphviz_shape_t::PARALLELOGRAM);
        }
    }
}