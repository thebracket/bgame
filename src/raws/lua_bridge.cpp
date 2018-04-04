#include "lua_bridge.hpp"
#include "../utils/system_log.hpp"

lua_State* lua_state;

void init_lua() {
	lua_state = luaL_newstate();
	
	// load Lua libraries
    static const luaL_Reg lualibs[] =
    {
        { "base", luaopen_base },
        { NULL, NULL}
    };

    const luaL_Reg *lib = lualibs;
    for(; lib->func != NULL; lib++)
    {
        lib->func(lua_state);
        lua_settop(lua_state, 0);
    }
}

void exit_lua() {
	lua_close(lua_state);
}

void load_lua_script(const std::string filename) {
	//std::cout << "Executing LUA: " << filename << "\n";
	if (luaL_loadfile(lua_state, filename.c_str())  || lua_pcall(lua_state, 0, 0, 0) ) {
		glog(log_target::LOADER, log_severity::error, "Lua error ({0}): {1}", filename, lua_tostring(lua_state, -1));
	}
}

inline void call_functor_key(const lua_parser &parser, const std::string &field, const std::string &table) {
    const auto finder = parser.find(field);
    if (finder != parser.end()) {
        const auto functor = finder->second;
        functor();
    } else {
        throw std::runtime_error(std::string("Lua - no known parser for [") + field + std::string("] in table: " + table));
    }    
}

void read_lua_table(const std::string &table, const std::function<void(std::string)> &on_start, const std::function<void(std::string)> &on_end, const lua_parser &parser) noexcept   
{
    lua_getglobal(lua_state, table.c_str());
    lua_pushnil(lua_state);
    while(lua_next(lua_state, -2) != 0)
    {
        const std::string key = lua_tostring(lua_state, -2);

        on_start(key);
        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            const std::string field = lua_tostring(lua_state, -2);
            call_functor_key(parser, field, table);
            lua_pop(lua_state, 1);
        }
        on_end(key);

        lua_pop(lua_state, 1);
    }
}

void read_lua_table_inner(const std::string &table, const std::function<void(std::string)> &functor)  noexcept
{
    lua_pushstring(lua_state, table.c_str());
    lua_gettable(lua_state, -2);
    while (lua_next(lua_state, -2) != 0) {
        const std::string s = lua_tostring(lua_state, -1);
        functor(s);
        lua_pop(lua_state, 1);
    }
}

void read_lua_table_inner_p(const std::string &table, const std::function<void(std::string)> &on_start, const std::function<void(std::string)> &on_end, const lua_parser &parser) noexcept
{
    lua_pushstring(lua_state, table.c_str());
    lua_gettable(lua_state, -2);
    while (lua_next(lua_state, -2) != 0) {
        const std::string key = lua_tostring(lua_state, -2);

        on_start(key);
        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            const std::string field = lua_tostring(lua_state, -2);
            call_functor_key(parser, field, table);
            lua_pop(lua_state, 1);
        }
        on_end(key);

        lua_pop(lua_state, 1);
    }
}

bengine::color_t read_lua_color(std::string field) noexcept {
    bengine::color_t col;
    lua_pushstring(lua_state, field.c_str());
    lua_gettable(lua_state, -2);
    while (lua_next(lua_state, -2) != 0) {
        const std::string subfield = lua_tostring(lua_state, -2);
        if (subfield == "r") col.r = static_cast<float>(lua_tonumber(lua_state, -1)) / 255.0f;
        if (subfield == "g") col.g = static_cast<float>(lua_tonumber(lua_state, -1)) / 255.0f;
        if (subfield == "b") col.b = static_cast<float>(lua_tonumber(lua_state, -1)) / 255.0f;
        lua_pop(lua_state, 1);
    }
    return col;
}

std::string lua_str_func(const std::string &func_name, const int &n) noexcept {
    std::string result;

    lua_getglobal(lua_state, func_name.c_str());
    lua_pushnumber(lua_state, n);
    if (lua_pcall(lua_state, 1, 1, 0) != 0) {
        //throw std::runtime_error(std::string("Error calling Lua function: ") + func_name + std::string(" ") + std::string(lua_tostring(lua_state, -1)));
    } else {
        result = lua_tostring(lua_state, -1);
    }
    lua_pop(lua_state, 1);
    return result;
}
