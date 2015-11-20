#pragma once

#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include "tile.h"

using std::vector;
using std::unordered_set;
using std::unordered_map;

constexpr int landblock_height = 256;
constexpr int landblock_width  = 256;
constexpr int tiles_per_landblock = landblock_height * landblock_width;

struct navigation_path {
  std::pair<int,int> destination;
  std::queue<std::pair<int,int>> steps;
};

class land_block {
public:
    land_block();
    land_block(int index_to_load);
    int idx(const int x, const int y) {
        return (y*landblock_width)+x;
    };

    int index;
    vector<tile> tiles;
    vector<bool> visible;
    vector<bool> revealed;

    void save();

    std::string get_filename() const;
    
    std::shared_ptr<navigation_path> find_path(std::pair<int,int> start, std::pair<int,int> end, unordered_map<int, bool> &walkable);
private:
    void load();
    void spread_flow( vector<std::tuple<short,int,int>> &new_cells, unordered_set<int> &visited, const int &x, const int &y, const short &distance, unordered_map<int, bool> &walkable );
};
