#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <queue>
#include <array>
#include <tuple>
#include <vector>
#include "../world/location.hpp"

using std::unordered_map;
using std::tuple;
using std::shared_ptr;
using std::queue;
using std::array;

namespace ai {
  
struct navigation_path {
  location_t destination;
  queue<location_t> steps;
};

shared_ptr<navigation_path> find_path(location_t start, location_t end);

  
}
