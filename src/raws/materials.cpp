#include "materials.hpp"
#include "lua_bridge.hpp"
#include "graphviz.hpp"
#include "defs/material_def_t.hpp"
#include <map>
#include <boost/container/flat_map.hpp>
#include <algorithm>
#include "../utils/system_log.hpp"

//using namespace rltk;

boost::container::flat_map<std::string, std::size_t> material_defs_idx;
std::vector<material_def_t> material_defs;
std::vector<std::string> material_textures;
std::vector<std::pair<std::string, std::string>> voxel_models_to_load;

/*
 * Retrieve a material by ID
 */
material_def_t * get_material(const std::size_t &idx) noexcept {
    if (idx < material_defs.size()) {
        return &material_defs[idx];
    }
    return nullptr;
}

/*
 * Retrieve a material's name
 */
std::string material_name(const std::size_t &id) noexcept {
    if (id < material_defs.size()) return material_defs[id].name;
    return std::string("Unknown material: ") + std::to_string(id);
}

/*
 * Retrieve a material by tag
 */
std::size_t get_material_by_tag(const std::string &tag) noexcept {
    const auto finder = material_defs_idx.find(tag);
    if (finder != material_defs_idx.end()) {
        return finder->second;
    }
    return 0;
}

/*
 * Retrieve all of the materials that can make up different strata layers
 */
void get_strata_materials(std::vector<std::size_t> &soils, std::vector<std::size_t> &sedimentaries,
                          std::vector<std::size_t> &igneouses, std::vector<std::size_t> &sands) noexcept
{
    std::size_t i = 0;
	for (auto &it : material_defs) {
        if (it.spawn_type == SOIL) soils.push_back(i);
        if (it.spawn_type == SAND) sands.push_back(i);
        if (it.spawn_type == ROCK && it.layer == "sedimentary") sedimentaries.push_back(i);
        if (it.spawn_type == ROCK && it.layer == "igneous") igneouses.push_back(i);
        ++i;
    }
}

/*
 * Check the validity of a material index #
 */
bool is_material_idx_valid(const std::size_t &id) noexcept
{
	if (id > material_defs.size()) {
		return false;
	}
	else {
		return true;
	}
}

/*
 * Linter for materials, used in raw loader.
 */
void sanity_check_materials() noexcept
{
    for (const auto &mat : material_defs) {
        if (mat.tag.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty material tag");
        if (mat.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Empty material name, tag: {0}", mat.tag);
        /*if (!mat.mines_to_tag.empty()) {
            auto finder = item_defs.find(mat.mines_to_tag);
            if (finder == item_defs.end()) std::cout << "WARNING: Unknown mining result " << mat.mines_to_tag << ", tag: " << mat.tag << "\n";
        }
        if (!mat.mines_to_tag_second.empty()) {
            auto finder = item_defs.find(mat.mines_to_tag_second);
            if (finder == item_defs.end()) std::cout << "WARNING: Unknown mining result " << mat.mines_to_tag_second << ", tag: " << mat.tag << "\n";
        }*/
        if (!mat.ore_materials.empty()) {
            for (const auto &metal : mat.ore_materials) {
                const auto finder = material_defs_idx.find(metal);
                if (finder == material_defs_idx.end()) {
					glog(log_target::LOADER, log_severity::warning, "WARNING: Substance {0} produces a non-existent ore: {1}", mat.tag, metal);
                }
            }
        }
    }
}

/*
 * Loads the material definitions from Lua.
 */
void read_material_types() noexcept
{
    lua_getglobal(lua_state, "materials");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        material_def_t m;
        m.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") m.name = lua_tostring(lua_state, -1);
            if (field == "type") {
                const std::string type_s = lua_tostring(lua_state, -1);
                if (type_s == "cluster_rock") {
                    m.spawn_type = CLUSTER_ROCK;
                } else if (type_s == "rock") {
                    m.spawn_type = ROCK;
                } else if (type_s == "soil") {
                    m.spawn_type = SOIL;
                } else if (type_s == "sand") {
                    m.spawn_type = SAND;
                } else if (type_s == "metal") {
                    m.spawn_type = METAL;
                } else if (type_s == "synthetic") {
                    m.spawn_type = SYNTHETIC;
                } else if (type_s == "organic") {
                    m.spawn_type = ORGANIC;
                } else if (type_s == "leather") {
                    m.spawn_type = LEATHER;
                } else if (type_s == "food") {
                    m.spawn_type = FOOD;
                } else if (type_s == "spice") {
                    m.spawn_type = SPICE;
                } else if (type_s == "blight") {
                    m.spawn_type = BLIGHT;
                } else {
					glog(log_target::LOADER, log_severity::warning, "WARNING: Unknown material type: {0}", type_s);
                }

            }
            if (field == "parent") m.parent_material_tag = lua_tostring(lua_state, -1);
            if (field == "glyph") m.glyph = static_cast<uint16_t>(lua_tonumber(lua_state, -1));
            if (field == "fg") m.fg = read_lua_color("fg");
            if (field == "bg") m.bg = read_lua_color("bg");
            if (field == "hit_points") m.hit_points = static_cast<uint8_t>(lua_tonumber(lua_state, -1));
            if (field == "mines_to") m.mines_to_tag = lua_tostring(lua_state, -1);
            if (field == "mines_to_also") m.mines_to_tag_second = lua_tostring(lua_state, -1);
            if (field == "layer") m.layer = lua_tostring(lua_state, -1);
            if (field == "ore_materials") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string metal_name = lua_tostring(lua_state, -1);
                    m.ore_materials.push_back(metal_name);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "damage_bonus") m.damage_bonus = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "ac_bonus") m.ac_bonus = static_cast<float>(lua_tonumber(lua_state, -1));
            if (field == "texture") m.base_texture_id = static_cast<int>(lua_tonumber(lua_state, -1));
            if (field == "constructed") m.constructed_texture_id = static_cast<int>(lua_tonumber(lua_state, -1));
			if (field == "floor") m.floor_texture_id = static_cast<int>(lua_tonumber(lua_state, -1));
			if (field == "floor_constructed") m.constructed_floor_texture_id = static_cast<int>(lua_tonumber(lua_state, -1));

            lua_pop(lua_state, 1);
        }
        material_defs.push_back(m);

        lua_pop(lua_state, 1);
    }

    std::sort(material_defs.begin(), material_defs.end(), [] (material_def_t a, material_def_t b) {
        return a.tag < b.tag;
    });
for (std::size_t material_index = 0; material_index < material_defs.size(); ++material_index) {
	material_defs_idx[material_defs[material_index].tag] = material_index;
}
}

void build_material_acquisition_tech_tree(graphviz_t *tree) {
	tree->add_trees();
	for (const auto &mat : material_defs) {
		switch (mat.spawn_type) {
		case NO_SPAWN_TYPE: tree->add_node("None", mat.tag); break;
		case CLUSTER_ROCK: tree->add_node("Cluster Rock", mat.tag); break;
		case ROCK: tree->add_node("Rock", mat.tag); break;
		case SOIL: tree->add_node("Soil", mat.tag); break;
		case SAND: tree->add_node("Sand", mat.tag); break;
		case METAL: tree->add_node("Metal", mat.tag); break;
		case SYNTHETIC: tree->add_node("Synthetic", mat.tag); break;
		case ORGANIC: tree->add_node("Organic", mat.tag); break;
		case LEATHER: tree->add_node("Leather", mat.tag); break;
		case FOOD: tree->add_node("Food", mat.tag); break;
		case SPICE: tree->add_node("Spice", mat.tag); break;
		case BLIGHT: tree->add_node("Blight", mat.tag); break;
		}
	}
}

void build_material_tech_tree(graphviz_t *tree) {
	/*for (const auto &mat : material_defs) {
		if (mat.mines_to_tag != "") {
			tree.add_node(mat.tag, std::string("item_") + mat.mines_to_tag);
		}
		if (mat.mines_to_tag_second != "") {
			tree.add_node(mat.tag, std::string("item_") + mat.mines_to_tag_second);
		}
	}*/
}

void read_material_textures() noexcept {
	std::map<int, std::string> tmp_tex;

	lua_getglobal(lua_state, "terrain_textures");
	lua_pushnil(lua_state);

	while (lua_next(lua_state, -2) != 0) {
		std::string key = lua_tostring(lua_state, -2);
		//std::cout << "Material texture key: " << key << "\n";

		auto idx = 0;
		std::string tex;

		lua_pushstring(lua_state, key.c_str());
		lua_gettable(lua_state, -2);
		while (lua_next(lua_state, -2) != 0) {
			const std::string field = lua_tostring(lua_state, -2);
			//std::cout << field << "\n";

			if (field == "index") idx = static_cast<int>(lua_tonumber(lua_state, -1));
			if (field == "texture") tex = lua_tostring(lua_state, -1);

			lua_pop(lua_state, 1);
		}
		tmp_tex[idx] = tex;

		lua_pop(lua_state, 1);
	}

	material_textures.clear();
	for (auto i = tmp_tex.begin(); i != tmp_tex.end(); ++i) {
		//std::cout << i->first << " == " << i->second << "\n";
		material_textures.emplace_back(i->second);
	}
}

void read_voxel_models() noexcept {
	lua_getglobal(lua_state, "voxel_models");
	lua_pushnil(lua_state);

	std::map<int, std::pair<std::string, std::string>> vox;

	while (lua_next(lua_state, -2) != 0) {
		std::string key = lua_tostring(lua_state, -2);
		//std::cout << key << "\n";

		auto idx = 0;
		std::string modelfile;

		lua_pushstring(lua_state, key.c_str());
		lua_gettable(lua_state, -2);
		while (lua_next(lua_state, -2) != 0) {
			const std::string field = lua_tostring(lua_state, -2);
			//std::cout << field << "\n";

			if (field == "model") modelfile = lua_tostring(lua_state, -1);
			if (field == "id") idx = static_cast<int>(lua_tonumber(lua_state, -1));

			lua_pop(lua_state, 1);
		}
		vox[idx] = std::make_pair(key, modelfile);

		lua_pop(lua_state, 1);

		voxel_models_to_load.clear();
		for (auto &it : vox) {
			voxel_models_to_load.emplace_back(it.second);
		}
	}
}
