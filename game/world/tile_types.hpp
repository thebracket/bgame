#pragma once

#include <cstdint>

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
constexpr unsigned char MAGMA = 17;
constexpr unsigned char EMPTY_SPACE = 18;
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
constexpr unsigned char SUBTERRANEAN = 9;
constexpr unsigned char MOLTEN = 10;
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

namespace biome_types {
constexpr uint8_t TUNDRA = 1;
constexpr uint8_t DESERT = 2;
constexpr uint8_t JUNGLE = 3;
constexpr uint8_t BADLANDS = 4;
constexpr uint8_t SWAMP = 5;
constexpr uint8_t FLATLAND = 6;
constexpr uint8_t WOODS = 7;
constexpr uint8_t FOREST = 8;
constexpr uint8_t HILLS = 9;
}
