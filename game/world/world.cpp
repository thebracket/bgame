#include "world.h"
#include "../tasks/help_wanted.h"

using std::pair;

namespace world {

array<unique_ptr<land_block>, world_size> world;
console log;
land_block * current_region;
std::vector<render_layers_t> entity_render_list ( tiles_per_landblock) ;
std::vector<bool> walk_blocked ( tiles_per_landblock );
std::vector<bool> view_blocked ( tiles_per_landblock );
unordered_map<string, vector<available_item>> inventory;

int camera_handle;
int cordex_handle;

string display_day_month;
string display_time;
string display_season;
float sun_angle = 0.0F;
int stored_power;
int max_power=100;
bool paused = true;
bool render_graphics = true;

void load_world_constants(fstream& lbfile)
{
    lbfile.read ( reinterpret_cast<char *> ( &camera_handle ), sizeof ( camera_handle ) );
    lbfile.read ( reinterpret_cast<char *> ( &cordex_handle ), sizeof ( cordex_handle ) );    
    lbfile.read ( reinterpret_cast<char *> ( &stored_power ), sizeof ( stored_power ) );    
    lbfile.read ( reinterpret_cast<char *> ( &render_graphics ), sizeof ( render_graphics ) );  
    
    ai::load_help_wanted( lbfile );
}

void save_world_constants(fstream& lbfile)
{
    lbfile.write ( reinterpret_cast<const char *> ( &camera_handle ), sizeof ( camera_handle ) );
    lbfile.write ( reinterpret_cast<const char *> ( &cordex_handle ), sizeof ( cordex_handle ) );
    lbfile.write ( reinterpret_cast<const char *> ( &stored_power ), sizeof ( stored_power ) );
    lbfile.write ( reinterpret_cast<const char *> ( &render_graphics ), sizeof ( render_graphics ) );
    
    ai::save_help_wanted( lbfile );
}


}

