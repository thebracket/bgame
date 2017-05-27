#include "models.hpp"
#include "lua_bridge.hpp"
#include "../utils/vox/voxreader.hpp"

void read_models() {
    lua_getglobal(lua_state, "voxmodels");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        const std::string tag = lua_tostring(lua_state, -2);
        const std::string filename = lua_tostring(lua_state, -1);
        vox::load_vox(filename, tag);

        lua_pop(lua_state, 1);
    }
}