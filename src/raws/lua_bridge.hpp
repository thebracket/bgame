#pragma once

extern "C" {
//#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <functional>
#include <string>
#include <unordered_map>
#include "../bengine/color_t.hpp"

extern lua_State* lua_state;

void init_lua();
void exit_lua();
void load_lua_script(std::string filename);

struct lua_lifecycle {
	lua_lifecycle() {
		init_lua();
	}

	~lua_lifecycle() {
		exit_lua();
	}
};

using lua_parser = std::unordered_map<std::string, const std::function<void()>>; 

void read_lua_table(const std::string &table, const std::function<void(std::string)> &on_start, const std::function<void(std::string)> &on_end, const lua_parser &parser);
void read_lua_table_inner(const std::string &table, const std::function<void(std::string)> &functor);
void read_lua_table_inner_p(const std::string &table, const std::function<void(std::string)> &on_start, const std::function<void(std::string)> &on_end, const lua_parser &parser);
inline std::string lua_str() { return lua_tostring(lua_state, -1); }
inline int lua_int() { return lua_tonumber(lua_state, -1); }
inline float lua_float() { return lua_tonumber(lua_state, -1); }
bengine::color_t read_lua_color(std::string field);

template<class T>
void read_stat_modifiers(const std::string &table, T &obj) {
    read_lua_table_inner( "modifiers", [&obj] (auto stat) {
        if (stat == "str") obj.strength = lua_int();
        if (stat == "dex") obj.dexterity = lua_int();
        if (stat == "con") obj.constitution = lua_int();
        if (stat == "int") obj.intelligence = lua_int();
        if (stat == "wis") obj.wisdom = lua_int();
        if (stat == "cha") obj.charisma = lua_int();
        if (stat == "com") obj.comeliness = lua_int();
        if (stat == "eth") obj.ethics = lua_int();
    });
}

std::string lua_str_func(const std::string &func_name, const int &n);