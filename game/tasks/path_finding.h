#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <queue>
#include <array>
#include "../world/landblock.h"

using std::unordered_map;
using std::pair;
using std::shared_ptr;
using std::queue;
using std::array;

namespace ai {
  
struct navigation_path {
  pair<int,int> destination;
  queue<std::pair<int,int>> steps;
};

shared_ptr<navigation_path> find_path(pair<int,int> start, pair<int,int> end, array<bool, tiles_per_landblock> &walkable);

  
}