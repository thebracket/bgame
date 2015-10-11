#include "world.h"

using std::pair;

namespace world {

array<unique_ptr<land_block>, world_size> world;
console log;
land_block * current_region;

int player_x;
int player_y;

}
