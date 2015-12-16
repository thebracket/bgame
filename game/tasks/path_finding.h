#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <queue>
#include <array>
#include <tuple>
#include "../world/landblock.h"

using std::unordered_map;
using std::tuple;
using std::shared_ptr;
using std::queue;
using std::array;

namespace ai {
  
struct navigation_path {
  tuple<int,int,int> destination;
  queue<std::tuple<int,int,int>> steps;
};

shared_ptr<navigation_path> find_path(tuple<int,int,int> start, tuple<int,int,int> end, vector<bool> &walkable);

  
}
