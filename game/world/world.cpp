#include "world.h"

using std::pair;

namespace world {

array<unique_ptr<land_block>, world_size> world;
console log;
land_block * current_region;
unordered_map<int, engine::vterm::screen_character> entity_render_list;
unordered_map<int, bool> walk_blocked;

int camera_handle;
int cordex_handle;

string display_day_month;
string display_time;
string display_season;
float sun_angle = 0.0F;
int stored_power;
int max_power=100;

void load_world_constants(fstream& lbfile)
{
    lbfile.read ( reinterpret_cast<char *> ( &camera_handle ), sizeof ( camera_handle ) );
    lbfile.read ( reinterpret_cast<char *> ( &cordex_handle ), sizeof ( cordex_handle ) );    
    lbfile.read ( reinterpret_cast<char *> ( &stored_power ), sizeof ( stored_power ) );    
}

void save_world_constants(fstream& lbfile)
{
    lbfile.write ( reinterpret_cast<const char *> ( &camera_handle ), sizeof ( camera_handle ) );
    lbfile.write ( reinterpret_cast<const char *> ( &cordex_handle ), sizeof ( cordex_handle ) );
    lbfile.write ( reinterpret_cast<const char *> ( &stored_power ), sizeof ( stored_power ) );
}


}
