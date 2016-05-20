#pragma once

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <string>

extern lua_State* lua_state;

void init_lua();
void exit_lua();
void load_lua_script(const std::string filename);

struct lua_lifecycle {
	lua_lifecycle() {
		init_lua();
	}

	~lua_lifecycle() {
		exit_lua();
	}
};

