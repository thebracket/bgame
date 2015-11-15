#include "render_tile_options.h"
#include "../world/world.h"
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;
using std::make_tuple;
using std::stringstream;

namespace render {

namespace tile_menu_details {

enum action_t { NONE };

typedef std::tuple<bool, int, unsigned char, engine::vterm::color_t, engine::vterm::color_t> menu_render_t;

struct tile_menu_option {
     string heading;
     menu_render_t icon;
     action_t action;
     // TODO: Action and sub-menu
};

class tile_menu {
public:
     tile_menu ( const int &world_idx, const std::pair<int,int> mouse_loc ) : idx ( world_idx ) {
          std::tie ( mouse_x, mouse_y ) = mouse_loc;
	  build_layer_options();
	  // Build heading
	  // Build options
     }

     void build_layer_options() {
       build_ground_layer();
       // Layer for settlers, objects present including in containers
     }
     
     inline void build_ground_layer() {
	  stringstream ss;
	  ss << world::current_region->tiles[idx].get_description() << " (" << world::current_region->tiles[idx].get_climate() << ")";
	  tile_menu_option ground {
	    ss.str(),
	    make_tuple( false, 0, 0, engine::vterm::color_t{0,0,0}, engine::vterm::color_t{0,0,0} ),
	    NONE
	  };
	  menu_options.push_back( ground );
     }
     
private:
     const int idx;
     int mouse_x, mouse_y;
     vector<tile_menu_option> menu_options;
     int width=0;
};

}

void tile_options ( sdl2_backend * SDL, const std::pair<int,int> region_loc, const std::pair<int,int> mouse_loc )
{
     const int idx = world::current_region->idx ( region_loc.first, region_loc.second );
     tile_menu_details::tile_menu menu ( idx, mouse_loc );
}

}