#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <queue>

using std::unordered_map;
using std::pair;
using std::shared_ptr;
using std::queue;

namespace ai {
  
struct navigation_path {
  pair<int,int> destination;
  queue<std::pair<int,int>> steps;
};

shared_ptr<navigation_path> find_path(pair<int,int> start, pair<int,int> end, unordered_map<int, bool> &walkable);

  
}