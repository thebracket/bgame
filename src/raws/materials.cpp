#include "materials.hpp"
#include "lua_bridge.hpp"
#include <boost/container/flat_map.hpp>
#include "apihelper.hpp"

using namespace rltk;

boost::container::flat_map<std::string, std::size_t> material_defs_idx;
std::vector<material_def_t> material_defs;

/*
 * Retrieve a material by ID
 */
boost::optional<material_def_t &> get_material(const std::size_t &idx) noexcept {
    boost::optional<material_def_t &> result;
    if (idx < material_defs.size()) {
        result = material_defs[idx];
    }
    return result;
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
boost::optional<std::size_t> get_material_by_tag(const std::string &tag) noexcept {
    boost::optional<std::size_t> result;

    auto finder = material_defs_idx.find(tag);
    if (finder != material_defs_idx.end()) {
        result = finder->second;
    }
    return result;
}

/*
 * Retrieve all of the materials that can make up different strata layers
 */
void get_strata_materials(std::vector<std::size_t> &soils, std::vector<std::size_t> &sedimentaries,
                          std::vector<std::size_t> &igneouses, std::vector<std::size_t> &sands) noexcept
{
    std::size_t i = 0;
    for (auto it=material_defs.begin(); it != material_defs.end(); ++it) {
        if (it->spawn_type == soil) soils.push_back(i);
        if (it->spawn_type == sand) sands.push_back(i);
        if (it->spawn_type == rock && it->layer == "sedimentary") sedimentaries.push_back(i);
        if (it->spawn_type == rock && it->layer == "igneous") igneouses.push_back(i);
        ++i;
    }
}

/*
 * Check the validity of a material index #
 */
bool is_material_idx_valid(const std::size_t &id) noexcept
{
    if (id > material_defs.size()) return false;
    return true;
}

/*
 * Linter for materials, used in raw loader.
 */
void sanity_check_materials() noexcept
{
    for (const auto &mat : material_defs) {
        if (mat.tag.empty()) std::cout << "WARNING: Empty material tag\n";
        if (mat.name.empty()) std::cout << "WARNING: Empty material name, tag: " << mat.tag << "\n";
        /*if (!mat.mines_to_tag.empty()) {
            auto finder = item_defs.find(mat.mines_to_tag);
            if (finder == item_defs.end()) std::cout << "WARNING: Unknown mining result " << mat.mines_to_tag << ", tag: " << mat.tag << "\n";
        }
        if (!mat.mines_to_tag_second.empty()) {
            auto finder = item_defs.find(mat.mines_to_tag_second);
            if (finder == item_defs.end()) std::cout << "WARNING: Unknown mining result " << mat.mines_to_tag_second << ", tag: " << mat.tag << "\n";
        }*/
        if (!mat.ore_materials.empty()) {
            for (const std::string &metal : mat.ore_materials) {
                auto finder = material_defs_idx.find(metal);
                if (finder == material_defs_idx.end()) {
                    std::cout << "WARNING: Substance " << mat.tag << " produces a non-existent ore: " << metal << "\n";
                }
            }
        }
    }
}

/*
 * Loads the material definitions from Lua.
 */
void read_material_types(std::ofstream &tech_tree_file) noexcept
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
                std::string type_s = lua_tostring(lua_state, -1);
                if (type_s == "cluster_rock") {
                    m.spawn_type = cluster_rock;
                } else if (type_s == "rock") {
                    m.spawn_type = rock;
                } else if (type_s == "soil") {
                    m.spawn_type = soil;
                } else if (type_s == "sand") {
                    m.spawn_type = sand;
                } else if (type_s == "metal") {
                    m.spawn_type = metal;
                } else if (type_s == "synthetic") {
                    m.spawn_type = synthetic;
                } else if (type_s == "organic") {
                    m.spawn_type = organic;
                } else if (type_s == "leather") {
                    m.spawn_type = leather;
                } else if (type_s == "food") {
                    m.spawn_type = food;
                } else if (type_s == "spice") {
                    m.spawn_type = spice;
                } else {
                    std::cout << "WARNING: Unknown material type: " << type_s << "\n";
                }

            }
            if (field == "parent") m.parent_material_tag = lua_tostring(lua_state, -1);
            if (field == "glyph") m.glyph = lua_tonumber(lua_state, -1);
            if (field == "fg") m.fg = read_lua_color("fg");
            if (field == "bg") m.bg = read_lua_color("bg");
            if (field == "hit_points") m.hit_points = lua_tonumber(lua_state, -1);
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
            if (field == "damage_bonus") m.damage_bonus = lua_tonumber(lua_state, -1);
            if (field == "ac_bonus") m.ac_bonus = lua_tonumber(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        material_defs.push_back(m);
        if (m.mines_to_tag.size() > 1)
            tech_tree_file << key << " -> mining -> item_" << m.mines_to_tag << "\n";
        if (m.mines_to_tag_second.size() > 1)
            tech_tree_file << key << " -> mining -> item_" << m.mines_to_tag_second << "\n";

        lua_pop(lua_state, 1);
    }

    std::sort(material_defs.begin(), material_defs.end(), [] (material_def_t a, material_def_t b) {
        return a.tag < b.tag;
    });
    for (std::size_t material_index = 0; material_index < material_defs.size(); ++material_index) {
        material_defs_idx[material_defs[material_index].tag] = material_index;
    }
}

