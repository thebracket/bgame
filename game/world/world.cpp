#include "world.h"

using std::pair;

namespace world {

array<unique_ptr<land_block>, world_size> world;
console log;
land_block * current_region;
map<int, engine::vterm::screen_character> entity_render_list;

int camera_handle;
int cordex_handle;

float sun_angle = 0.0F;

void load_world_constants(fstream& lbfile)
{
    lbfile.read ( reinterpret_cast<char *> ( &camera_handle ), sizeof ( camera_handle ) );
    lbfile.read ( reinterpret_cast<char *> ( &cordex_handle ), sizeof ( cordex_handle ) );    
}

void save_world_constants(fstream& lbfile)
{
    lbfile.write ( reinterpret_cast<const char *> ( &camera_handle ), sizeof ( camera_handle ) );
    lbfile.write ( reinterpret_cast<const char *> ( &cordex_handle ), sizeof ( cordex_handle ) );
}


}
