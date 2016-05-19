#pragma once

#include <string>

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

