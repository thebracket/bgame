#include "reactions.hpp"
#include "lua_bridge.hpp"
#include "materials.hpp"
#include "buildings.hpp"
#include "items.hpp"
#include "clothing.hpp"

std::unordered_map<std::string, reaction_t> reaction_defs;
std::unordered_map<std::string, std::vector<std::string>> reaction_building_defs;

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

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "workshop") c.workshop = lua_tostring(lua_state, -1);
            if (field == "skill") c.skill = lua_tostring(lua_state, -1);
            if (field == "difficulty") c.difficulty = lua_tonumber(lua_state, -1);
            if (field == "inputs") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);

                    reaction_input_t input;
                    input.required_material = 0;
                    input.required_material_type = no_spawn_type;
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") input.tag = lua_tostring(lua_state, -1);
                        if (f == "qty") input.quantity = lua_tonumber(lua_state, -1);
                        if (f == "material") {
                            std::string mat_name = lua_tostring(lua_state, -1);
                            auto matfinder = get_material_by_tag(mat_name);
                            if (matfinder == 0) {
                                std::cout << "WARNING: Reaction " << c.name << " references unknown material " << mat_name << "\n";
                            } else {
                                input.required_material = matfinder;
                            }
                        }
                        if (f == "mat_type") {
                            std::string type_s = lua_tostring(lua_state, -1);
                            if (type_s == "cluster_rock") {
                                input.required_material_type = cluster_rock;
                            } else if (type_s == "rock") {
                                input.required_material_type = rock;
                            } else if (type_s == "soil") {
                                input.required_material_type = soil;
                            } else if (type_s == "sand") {
                                input.required_material_type = sand;
                            } else if (type_s == "metal") {
                                input.required_material_type = metal;
                            } else if (type_s == "synthetic") {
                                input.required_material_type = synthetic;
                            } else if (type_s == "organic") {
                                input.required_material_type = organic;
                            } else if (type_s == "leather") {
                                input.required_material_type = leather;
                            } else if (type_s == "food") {
                                input.required_material_type = food;
                            } else if (type_s == "spice") {
                                input.required_material_type = spice;
                            } else {
                                std::cout << "WARNING: Unknown material type: " << type_s << "\n";
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
                        std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") comp.first = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.second = lua_tonumber(lua_state, -1);
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
            if (field == "power_drain") c.power_drain = lua_tonumber(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        reaction_defs[key] = c;
        reaction_building_defs[c.workshop].push_back(key);

        lua_pop(lua_state, 1);
    }
}

void sanity_check_reactions() noexcept
{
    for (auto it=reaction_defs.begin(); it!=reaction_defs.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Empty reaction name\n";
        if (it->second.tag.empty()) std::cout << "WARNING: Empty reaction tag\n";
        if (it->second.name.empty()) std::cout << "WARNING: Empty reaction name, tag: " << it->first << "\n";
        if (it->second.workshop.empty()) std::cout << "WARNING: Empty workshop name, tag: " << it->first << "\n";
        auto bf = get_building_def(it->second.workshop);
        if (bf == nullptr) std::cout << "WARNING: Undefined workshop, tag: " << it->first << "\n";
        for (const auto &input : it->second.inputs) {
            auto finder = item_defs.find(input.tag);
            if (finder == item_defs.end()) std::cout << "WARNING: Unknown item tag in input: " << input.tag << ", reaction tag: " << it->first << "\n";
        }
        for (const auto &output : it->second.outputs) {
            auto finder = item_defs.find(output.first);
            auto finder2 = get_clothing_by_tag(output.first);
            if (finder == item_defs.end() && !finder2) std::cout << "WARNING: Unknown item tag in output: " << output.first << ", reaction tag: " << it->first << "\n";
        }
    }
}

void build_reaction_tree(graphviz_t &tree) {
    for (auto it=reaction_defs.begin(); it!=reaction_defs.end(); ++it) {
        tree.add_node(std::string("building_") + it->second.workshop, it->second.tag, graphviz_t::graphviz_shape_t::HOUSE);
        for (const auto &input : it->second.inputs) {
            tree.add_node(std::string("item_") + input.tag, it->second.tag);
        }
        for (const auto &output : it->second.outputs) {
            tree.add_node(it->second.tag, std::string("item_") + output.first, graphviz_t::graphviz_shape_t::PARALLELOGRAM);
        }
    }
}