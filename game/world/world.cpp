#include "world.h"

using std::pair;

namespace world {

array<unique_ptr<land_block>, world_size> world;
console log;
land_block * current_region;
map<int, engine::vterm::screen_character> entity_render_list;

int camera_handle;
int cordex_handle;
}
