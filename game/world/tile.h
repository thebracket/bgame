#pragma once

#include "../../engine/virtual_terminal.h"
#include "tile_types.hpp"
#include <fstream>

using engine::vterm::color_t;
using std::fstream;

class tile {
public:
    tile();
    
    inline bool is_ramp() {
	if (base_tile_type == tile_type::RAMP or base_tile_type > 4) {
	  return true;
	} else {
	  return false;
	}
    }

    // About the tile itself
    unsigned char base_tile_type;
    short altitude; // Only used in worldgen
    short temperature; // Only used in worldgen
    unsigned char level_band;
    unsigned char climate;
    unsigned char ground;
    unsigned char covering;

    void worldgen_tile_calc();
    void calculate_display_properties();

    // Cached: not persistent
    unsigned char display;
    color_t foreground {0,255,0};
    color_t background {0,0,0};

    void save(fstream &lbfile);
    void load(fstream &lbfile);
    
    std::string get_climate();
    std::string get_description();
    
private:
  void worldgen_determine_climate();
  void worldgen_determine_base_ground();
  void worldgen_arctic_covering();
  void worldgen_subarctic_covering();
  void worldgen_cold_covering();
  void worldgen_cool_covering();
  void worldgen_temperate_covering();
  void worldgen_warm_covering();
  void worldgen_hot_covering();
  void worldgen_very_hot_covering();
};

