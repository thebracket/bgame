#pragma once

#include <mutex>
#include <vector>
#include <memory>
#include "landblock.h"

using std::shared_ptr;
using std::vector;

namespace worldgen {

extern std::mutex worldgen_mutex;
enum worldgen_progress { INIT, FAULTLINES, MAKETILES, DONE };
extern worldgen_progress progress;

extern vector<short> * heightmap_progress;
extern land_block * current_landblock;
  
void build_world();

}

