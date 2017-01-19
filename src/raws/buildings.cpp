#include "buildings.hpp"
#include "lua_bridge.hpp"
#include "items.hpp"
#include "materials.hpp"

using namespace rltk;

boost::container::flat_map<std::string, building_def_t> building_defs;

void sanity_check_buildings() noexcept
{
    for (auto it=building_defs.begin(); it!=building_defs.end(); ++it) {
        if (it->first.empty()) std::cout << "WARNING: Empty building tag\n";
        if (it->second.name.empty()) std::cout << "WARNING: Building " << it->first << " has no name.\n";
        for (const reaction_input_t &comp : it->second.components) {
            if (comp.tag.empty()) std::cout << "WARNING: Empty component for building: " << it->first << "\n";
            auto finder = item_defs.find(comp.tag);
            if (finder == item_defs.end()) {
                std::cout << "WARNING: No item definition for component " << comp.tag << ", for building: " << it->first << "\n";
            }
        }
    }
}

void read_buildings(std::ofstream &tech_tree_file) noexcept
{
    lua_getglobal(lua_state, "buildings");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        building_def_t c;

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "structure") c.structure = true;
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "components") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {

                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);

                    reaction_input_t comp;
                    comp.required_material = boost::optional<std::size_t>{};
                    comp.required_material_type = boost::optional<material_def_spawn_type_t>{};
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);

                        if (f == "item") comp.tag = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.quantity = lua_tonumber(lua_state, -1);
                        if (f == "material") {
                            std::string mat_name = lua_tostring(lua_state, -1);
                            auto matfinder = get_material_by_tag(mat_name);
                            if (!matfinder) {
                                std::cout << "WARNING: Reaction " << c.name << " references unknown material " << mat_name << "\n";
                            } else {
                                comp.required_material = matfinder.get();
                            }
                        }
                        if (f == "mat_type") {
                            std::string type_s = lua_tostring(lua_state, -1);
                            if (type_s == "cluster_rock") {
                                comp.required_material_type = cluster_rock;
                            } else if (type_s == "rock") {
                                comp.required_material_type = rock;
                            } else if (type_s == "soil") {
                                comp.required_material_type = soil;
                            } else if (type_s == "sand") {
                                comp.required_material_type = sand;
                            } else if (type_s == "metal") {
                                comp.required_material_type = metal;
                            } else if (type_s == "synthetic") {
                                comp.required_material_type = synthetic;
                            } else if (type_s == "organic") {
                                comp.required_material_type = organic;
                            } else if (type_s == "leather") {
                                comp.required_material_type = leather;
                            } else {
                                std::cout << "WARNING: Unknown material type: " << type_s << "\n";
                            }
                        }
                        lua_pop(lua_state, 1);
                    }

                    c.components.push_back(comp);
                    tech_tree_file << "item_" << comp.tag << " -> " <<  key << "\n";
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "skill") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "name") c.skill.first = lua_tostring(lua_state, -1);
                    if (type == "difficulty") c.skill.second = lua_tonumber(lua_state, -1);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "provides") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);

                while (lua_next(lua_state, -2) != 0) {
                    building_provides_t provisions;
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "table") provisions.provides = provides_desk;
                    if (type == "wall") provisions.provides = provides_wall;
                    if (type == "door") provisions.provides = provides_door;
                    if (type == "food") provisions.provides = provides_food;
                    if (type == "sleep") provisions.provides = provides_sleep;
                    if (type == "floor") provisions.provides = provides_floor;
                    if (type == "stairs_up") provisions.provides = provides_stairs_up;
                    if (type == "stairs_down") provisions.provides = provides_stairs_down;
                    if (type == "stairs_updown") provisions.provides = provides_stairs_updown;
                    if (type == "ramp") provisions.provides = provides_ramp;
                    if (type == "light") provisions.provides = provides_light;
                    if (type == "cage_trap") provisions.provides = provides_cage_trap;
                    if (type == "stonefall_trap") provisions.provides = provides_stonefall_trap;

                    lua_pushstring(lua_state, type.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string inner_type = lua_tostring(lua_state, -2);
                        if (inner_type == "energy_cost") provisions.energy_cost = lua_tonumber(lua_state, -1);
                        if (inner_type == "radius") provisions.radius = lua_tonumber(lua_state, -1);
                        if (inner_type == "color") provisions.color = read_lua_color("color");
                        lua_pop(lua_state, 1);
                    }

                    c.provides.push_back(provisions);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "width") c.width = lua_tonumber(lua_state, -1);
                    if (type == "height") c.height = lua_tonumber(lua_state, -1);
                    if (type == "tiles") {
                        lua_pushstring(lua_state, type.c_str());
                        lua_gettable(lua_state, -2);
                        int i = 0;
                        while (lua_next(lua_state, -2) != 0) {
                            rltk::vchar render;
                            lua_pushnumber(lua_state, i);
                            lua_gettable(lua_state, -2);
                            while (lua_next(lua_state, -2) != 0) {
                                std::string tiletag = lua_tostring(lua_state, -2);
                                if (tiletag == "glyph") render.glyph = lua_tonumber(lua_state, -1);
                                if (tiletag == "foreground") render.foreground = read_lua_color("foreground");
                                if (tiletag == "background") render.background = read_lua_color("background");
                                lua_pop(lua_state, 1);
                            }
                            lua_pop(lua_state, 1);
                            ++i;
                            c.glyphs.push_back(render);
                        }
                    }
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render_rex") {
                std::string filename = "rex/" + std::string(lua_tostring(lua_state, -1));
                xp::rex_sprite sprite(filename);
                c.width = sprite.get_width();
                c.height = sprite.get_height();
                for (int y=0; y<c.height; ++y) {
                    for (int x=0; x<c.width; ++x) {
                        c.glyphs.push_back(*sprite.get_tile(0,x,y));
                    }
                }
            }

            lua_pop(lua_state, 1);
        }
        building_defs[key] = c;
        lua_pop(lua_state, 1);
    }
}
