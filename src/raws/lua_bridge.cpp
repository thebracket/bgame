#include "lua_bridge.hpp"
#include <iostream>

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
		std::cout << lua_tostring(lua_state, -1) << "\n";
	}
}