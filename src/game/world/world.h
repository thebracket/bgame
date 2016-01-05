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

extern std::vector<optional<engine::vterm::screen_character>> render_list_3d;

}
