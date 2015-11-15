#include "render_tooltips.h"
#include "world/world.h"
#include "game.h"
#include "raws/raws.h"
#include "colors.h"
#include <tuple>
#include <sstream>

using std::make_pair;
using std::pair;
using std::vector;
using std::string;
using std::tuple;
using std::stringstream;

typedef vector<pair<string, SDL_Color>> line_buffer_t;

namespace tooltip_detail {

class tip {
public:
     tip ( const int &sx, const int &sy ) : screen_x ( sx ), screen_y ( sy ) {}

     void add_line ( const string &text, const SDL_Color &col ) {
          lines.push_back ( make_pair ( text, col ) );
          const int text_size = text.size() * 7;
          if ( text_size > width ) width = text_size;
     }

     void render ( sdl2_backend * SDL ) {
          int y = screen_y - ( ( lines.size() /2 ) * 16 );

          SDL->set_alpha_mod ( "spritesheet", 128 );
          SDL_Rect source = raws::get_tile_source_by_name ( "BLACKMASK" );
          const unsigned char height = lines.size() *16;
          SDL_Rect dest = { screen_x, y-4, width, height+8 };
          SDL->render_bitmap ( "spritesheet", source, dest );
          SDL->set_alpha_mod ( "spritesheet", 255 );

          for ( const std::pair<std::string,SDL_Color> &tooltip_line : lines ) {
               std::string line_s = SDL->render_text_to_image ( "disco14", tooltip_line.first, "tmp", tooltip_line.second );
               SDL->render_bitmap_simple ( line_s, screen_x + 16, y );
               y += 16;
          }

     }

private:
     const int screen_x;
     const int screen_y;
     line_buffer_t lines;
     int width = 0;

     inline bool add_settler_details ( const int entity_id ) {
          const settler_ai_component * settler = game_engine->ecs->find_entity_component<settler_ai_component> ( entity_id );
          if ( settler == nullptr ) return false;

          stringstream settler_name;
          settler_name << settler->first_name << " " << settler->last_name << " (" << settler->profession_tag << ")";
          add_line ( settler_name.str(), render::sdl_white );

          return true;
     }

     inline void add_name_details ( const int entity_id ) {
          const debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component> ( entity_id );
          if ( name != nullptr ) {
               add_line ( name->debug_name, render::sdl_white );
          }
     }

     inline void add_containers ( const int entity_id ) {
          vector<item_storage_component *> stored_items = game_engine->ecs->find_components_by_func<item_storage_component> (
          [entity_id] ( const item_storage_component &e ) {
               if ( e.container_id == entity_id ) {
                    return true;
               } else {
                    return false;
               }
          } );
          for ( item_storage_component * item : stored_items ) {
               debug_name_component * nc = game_engine->ecs->find_entity_component<debug_name_component> ( item->entity_id );
               add_line ( string ( " " ) + nc->debug_name, render::sdl_green );
          }
     }

public:
     void add_tile_contents ( const int region_x, const int region_y ) {
          vector<position_component *> positions = game_engine->ecs->find_components_by_func<position_component> (
          [region_x, region_y] ( const position_component &c ) {
               return ( c.x == region_x and c.y == region_y );
          }
                    );
          for ( const position_component * pos : positions ) {
               const int entity_id = pos->entity_id;
               if ( !add_settler_details ( entity_id ) ) add_name_details ( entity_id );
               add_containers ( entity_id );
          }
     }

};

}

void render::tooltip ( sdl2_backend * SDL, const std::pair<int,int> region_loc, const std::pair<int,int> mouse_loc )
{
     int region_x, region_y, mouse_vx, mouse_vy;
     std::tie ( region_x, region_y ) = region_loc;
     std::tie ( mouse_vx, mouse_vy ) = mouse_loc;

     const int idx = world::current_region->idx ( region_x, region_y );
     if ( world::current_region->visible[ idx ] == false ) {
          return;
     }

     const int screen_x = mouse_vx*16 + 16;
     const int screen_y = mouse_vy*16 + 48;

     tooltip_detail::tip tt ( screen_x, screen_y );
     tt.add_line ( world::current_region->tiles[idx].get_description(), sdl_green );
     tt.add_line ( world::current_region->tiles[idx].get_climate(), sdl_cyan );
     tt.add_tile_contents ( region_x, region_y );
     tt.render ( SDL );
}