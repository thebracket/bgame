#include "buildings.hpp"
#include "lua_bridge.hpp"
#include "items.hpp"
#include "materials.hpp"
#include "defs/building_def_t.hpp"
#include "graphviz.hpp"
#include "../bengine/rexspeeder.hpp"
#include <boost/container/flat_map.hpp>
#include "../utils/system_log.hpp"

//using namespace rltk;

boost::container::flat_map<std::size_t, building_def_t> building_defs;

building_def_t * get_building_def(const std::string &tag) noexcept {
	auto finder = building_defs.find(std::hash<std::string>{}(tag));
    if (finder == building_defs.end()) return nullptr;
    return &finder->second;
}

void each_building_def(const std::function<void(building_def_t *)> &func) noexcept {
	for (auto &it : building_defs) {
        func(&it.second);
    }
}

void sanity_check_buildings() noexcept
{
	for (auto &it : building_defs) {
        if (it.second.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Building {0} has no name.\n", it.first);
        for (const auto &comp : it.second.components) {
            if (comp.tag.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty component for building: {0}", it.first);
            const auto finder = get_item_def(comp.tag);
            if (finder == nullptr) {
				glog(log_target::LOADER, log_severity::warning, "WARNING: No item definition for component {0}, for building {1}", comp.tag, it.first);
            }
        }
        //if (it->second.glyphs_ascii.size() != it->second.glyphs.size()) std::cout << "WARNING: Building " << it->first << " has invalid ASCII render data.\n";
    }
}

void read_buildings() noexcept
{
    lua_getglobal(lua_state, "buildings");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        building_def_t c;

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;
		c.hashtag = std::hash<std::string>{}(c.tag);

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
			if (field == "description") c.description = lua_tostring(lua_state, -1);
			if (field == "blocked") c.blocked = lua_tostring(lua_state, -1);
			if (field == "vox") c.vox_model = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "structure") c.structure = true;
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "components") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {

                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);

                    reaction_input_t comp;
                    comp.required_material = 0;
                    comp.required_material_type = NO_SPAWN_TYPE;
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);

                        if (f == "item") comp.tag = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.quantity = static_cast<int>(lua_tonumber(lua_state, -1));
                        if (f == "material") {
                            const std::string mat_name = lua_tostring(lua_state, -1);
                            const auto matfinder = get_material_by_tag(mat_name);
                            if (matfinder == 0) {
								glog(log_target::LOADER, log_severity::warning, "WARNING: Reaction %s references unknown material {0}", c.name, mat_name);
                            } else {
                                comp.required_material = matfinder;
                            }
                        }
                        if (f == "mat_type") {
                            const std::string type_s = lua_tostring(lua_state, -1);
                            if (type_s == "cluster_rock") {
                                comp.required_material_type = CLUSTER_ROCK;
                            } else if (type_s == "rock") {
                                comp.required_material_type = ROCK;
                            } else if (type_s == "soil") {
                                comp.required_material_type = SOIL;
                            } else if (type_s == "sand") {
                                comp.required_material_type = SAND;
                            } else if (type_s == "metal") {
                                comp.required_material_type = METAL;
                            } else if (type_s == "synthetic") {
                                comp.required_material_type = SYNTHETIC;
                            } else if (type_s == "organic") {
                                comp.required_material_type = ORGANIC;
                            } else if (type_s == "leather") {
                                comp.required_material_type = LEATHER;
                            } else {
								glog(log_target::LOADER, log_severity::warning, "Unknown material type {0}", type_s);
                            }
                        }
                        lua_pop(lua_state, 1);
                    }

                    c.components.push_back(comp);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "skill") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string type = lua_tostring(lua_state, -2);
                    if (type == "name") c.skill.first = lua_tostring(lua_state, -1);
                    if (type == "difficulty") c.skill.second = static_cast<int>(lua_tonumber(lua_state, -1));
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
                    if (type == "blade_trap") provisions.provides = provides_blades_trap;
                    if (type == "spike_trap") provisions.provides = provides_spikes;
                    if (type == "lever") provisions.provides = provides_lever;
					if (type == "pressure_plate") provisions.provides = provides_pressure_plate;
					if (type == "signal") provisions.provides = provides_signal_recipient;
					if (type == "storage") provisions.provides = provides_storage;
					if (type == "oscillator") provisions.provides = provides_oscillator;
					if (type == "gate_and") provisions.provides = provides_and_gate;
					if (type == "gate_or") provisions.provides = provides_or_gate;
					if (type == "gate_not") provisions.provides = provides_not_gate;
					if (type == "gate_nand") provisions.provides = provides_nand_gate;
					if (type == "gate_nor") provisions.provides = provides_nor_gate;
					if (type == "gate_xor") provisions.provides = provides_xor_gate;
					if (type == "float_sensor") provisions.provides = provides_float_gauge;
					if (type == "proximity_sensor") provisions.provides = provides_proximity_sensor;
					if (type == "support") provisions.provides = provides_support;

                    lua_pushstring(lua_state, type.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string inner_type = lua_tostring(lua_state, -2);
                        if (inner_type == "energy_cost") provisions.energy_cost = static_cast<int>(lua_tonumber(lua_state, -1));
                        if (inner_type == "radius") provisions.radius = static_cast<int>(lua_tonumber(lua_state, -1));
                        if (inner_type == "color") provisions.color = read_lua_color("color");
						if (inner_type == "alternate_vox") provisions.alternate_vox = static_cast<int>(lua_tonumber(lua_state, -1));
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
                    if (type == "width") c.width = static_cast<int>(lua_tonumber(lua_state, -1));
                    if (type == "height") c.height = static_cast<int>(lua_tonumber(lua_state, -1));
                    if (type == "tiles") {
                        lua_pushstring(lua_state, type.c_str());
                        lua_gettable(lua_state, -2);
                        auto i = 0;
                        while (lua_next(lua_state, -2) != 0) {
                            xp::vchar render;
                            lua_pushnumber(lua_state, i);
                            lua_gettable(lua_state, -2);
                            while (lua_next(lua_state, -2) != 0) {
                                const std::string tiletag = lua_tostring(lua_state, -2);
                                if (tiletag == "glyph") render.glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
                                if (tiletag == "foreground") render.foreground = read_lua_color("foreground");
                                if (tiletag == "background") render.background = read_lua_color("background");
                                lua_pop(lua_state, 1);
                            }
                            ++i;
                            c.glyphs.push_back(render);                             
                            lua_pop(lua_state, 1);
                        }
                    }
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render_ascii") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "width") c.width = static_cast<int>(lua_tonumber(lua_state, -1));
                    if (type == "height") c.height = static_cast<int>(lua_tonumber(lua_state, -1));
                    if (type == "tiles") {
                        lua_pushstring(lua_state, type.c_str());
                        lua_gettable(lua_state, -2);
                        int i = 0;
                        while (lua_next(lua_state, -2) != 0) {
                            xp::vchar render;
                            lua_pushnumber(lua_state, i);
                            lua_gettable(lua_state, -2);
                            while (lua_next(lua_state, -2) != 0) {
                                const auto tiletag = std::string(lua_tostring(lua_state, -2));
                                if (tiletag == "glyph") render.glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
                                if (tiletag == "foreground") render.foreground = read_lua_color("foreground");
                                if (tiletag == "background") render.background = read_lua_color("background");
                                lua_pop(lua_state, 1);
                            }
                            ++i;
                            c.glyphs_ascii.push_back(render);
                            lua_pop(lua_state, 1);
                        }
                    }
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render_rex") {
                const auto filename = std::string("game_assets/rex/") + std::string(lua_tostring(lua_state, -1));
                xp::rex_sprite sprite(filename);
                c.width = sprite.get_width();
                c.height = sprite.get_height();
                for (auto y=0; y<c.height; ++y) {
                    for (auto x=0; x<c.width; ++x) {
						const auto tmp = *sprite.get_tile(0, x, y);
                        c.glyphs.push_back(tmp);
                        c.glyphs_ascii.push_back(tmp);
                    }
                }
                //std::cout << "Loaded REX file: " << filename << ", " << c.width << " x " << c.height << "\n";
            }

            lua_pop(lua_state, 1);
        }
        building_defs[c.hashtag] = c;
        //std::cout << "Read schematics for building: " << key << " (VOX " << building_defs[key].vox_model << ")\n";
        lua_pop(lua_state, 1);
    }
}

void make_building_tree(graphviz_t * tree) {
    for (const auto &b : building_defs) {
        for (const auto &input : b.second.components) {
            tree->add_node(std::string("item_") + input.tag, std::string("building_") + b.second.tag );
        }
    }
}
