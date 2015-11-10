#pragma once

#include "../../engine/virtual_terminal.h"
#include <fstream>

using engine::vterm::color_t;
using std::fstream;

namespace tile_type {
constexpr unsigned char WATER = 1;
constexpr unsigned char FLAT = 2;
constexpr unsigned char RAMP = 3;
constexpr unsigned char BEACH = 4;
constexpr unsigned char RAMP_NU_SD = 5;
constexpr unsigned char RAMP_ND_SU = 6;
constexpr unsigned char RAMP_EU_WD = 7;
constexpr unsigned char RAMP_ED_WU = 8;
constexpr unsigned char RAMP_WU_SU = 9;
constexpr unsigned char RAMP_EU_SU = 10;
constexpr unsigned char RAMP_EU_NU = 11;
constexpr unsigned char RAMP_WU_NU = 12;
constexpr unsigned char RAMP_WD_ND = 13;
constexpr unsigned char RAMP_ED_ND = 14;
constexpr unsigned char RAMP_ED_SD = 15;
constexpr unsigned char RAMP_WD_SD = 16;
}

namespace tile_climate {
constexpr unsigned char ARCTIC = 1;
constexpr unsigned char SUBARCTIC = 2;
constexpr unsigned char COLD = 3;
constexpr unsigned char COOL = 4;
constexpr unsigned char TEMPERATE = 5;
constexpr unsigned char WARM = 6;
constexpr unsigned char HOT = 7;
constexpr unsigned char VERY_HOT = 8;
}

namespace tile_ground {
constexpr unsigned char IGNEOUS = 1;
constexpr unsigned char SEDIMENTARY = 2;
constexpr unsigned char GRAVEL = 3;
constexpr unsigned char WHITE_SAND = 4;
constexpr unsigned char YELLOW_SAND = 5;
constexpr unsigned char RED_SAND = 6;
}

namespace tile_covering {
constexpr unsigned char BARE = 0;
constexpr unsigned char MOSS = 1;
constexpr unsigned char LYCHEN = 2;
constexpr unsigned char HEATHER = 3;
constexpr unsigned char GRASS = 4;
constexpr unsigned char SHRUB = 5;
constexpr unsigned char THISTLE = 6;
constexpr unsigned char GORSE = 7;
constexpr unsigned char REEDS = 8;
constexpr unsigned char WILDFLOWER = 9;
constexpr unsigned char CACTUS = 10;
}

class tile {
public:
    tile();

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

