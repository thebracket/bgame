#pragma once

#include <vector>
#include "tile.h"

using std::vector;

constexpr int landblock_height = 256;
constexpr int landblock_width  = 256;
constexpr int tiles_per_landblock = landblock_height * landblock_width;

class land_block {
public:
  land_block();
  int idx(const int x, const int y) { return (y*landblock_width)+x; };
  
  void make_radius_visible ( const int x, const int y, const int radius );
  void reset_visibility();

  int index;
  vector<tile> tiles;
};