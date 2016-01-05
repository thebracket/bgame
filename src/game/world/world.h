#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <string>
#include <tuple>
#include <array>
#include "../../engine/virtual_terminal.hpp"
#include <boost/optional.hpp>
#include "location.hpp"

using std::array;
using std::unique_ptr;
using std::unordered_map;
using std::fstream;
using std::string;
using std::tuple;
using boost::optional;

namespace world
{

// Convenience: storing the camera's handle globally is messy,
// but makes it quick and easy for systems that want to find it.
extern int max_power;
extern bool paused;
extern bool render_graphics;
extern bool omniscience;

extern std::vector<optional<engine::vterm::screen_character>> render_list_3d;

struct available_item
{
	string name;
	location_t location;
	int entity_id;
};
extern unordered_map<string, vector<available_item>> inventory;

}
