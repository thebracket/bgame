#include "render_tooltips.h"
#include "world/world.h"
#include "game.h"
#include "raws/raws.h"

using std::make_pair;

void render::tooltip ( sdl2_backend * SDL, const std::pair<int,int> region_loc, const std::pair<int,int> mouse_loc )
{

     constexpr SDL_Color sdl_green { 64,255,64,0 };
     constexpr SDL_Color sdl_cyan { 0,255,255,0 };
     constexpr SDL_Color sdl_white { 255,255,255,0 };

     int region_x, region_y, mouse_vx, mouse_vy;
     std::tie ( region_x, region_y ) = region_loc;
     std::tie ( mouse_vx, mouse_vy ) = mouse_loc;

     const int idx = world::current_region->idx ( region_x, region_y );
     if ( world::current_region->visible[ idx ] == false ) {
          return;
     }

     const int screen_x = mouse_vx*16 + 16;
     const int screen_y = mouse_vy*16 + 48;

     std::vector<std::pair<std::string,SDL_Color>> lines;

     lines.push_back ( make_pair ( world::current_region->tiles[idx].get_description(), sdl_green ) );
     lines.push_back ( make_pair ( world::current_region->tiles[idx].get_climate(), sdl_cyan ) );

     vector<position_component> * positions = game_engine->ecs->find_components_by_type<position_component>();
     for ( const position_component &pos : *positions ) {
          if ( pos.x == region_x and pos.y == region_y ) {
               const int entity_id = pos.entity_id;
               std::stringstream desc;
               settler_ai_component * settler = game_engine->ecs->find_entity_component<settler_ai_component> ( entity_id );
               if ( settler != nullptr ) {
                    //game_species_component * species = game_engine->ecs->find_entity_component<game_species_component> ( entity_id );
                    desc << settler->first_name << " " << settler->last_name << " (" << settler->profession_tag << ")";
                    lines.push_back ( make_pair ( desc.str(), sdl_white ) );
               } else {
                    debug_name_component * debug_name = game_engine->ecs->find_entity_component<debug_name_component> ( entity_id );
                    if ( debug_name != nullptr ) {
                         desc << debug_name->debug_name;
                         lines.push_back ( make_pair ( desc.str(), sdl_white ) );
                    }

                    // Stored items
                    vector<item_storage_component *> stored_items = game_engine->ecs->find_components_by_func<item_storage_component> (
                    [entity_id] ( const item_storage_component &e ) {
                         if ( e.container_id == entity_id ) {
                              return true;
                         }
                         return false;
                    }
                              );
                    for ( item_storage_component * item : stored_items ) {
                         debug_name_component * nc = game_engine->ecs->find_entity_component<debug_name_component> ( item->entity_id );
                         lines.push_back ( make_pair ( string ( "   " ) + nc->debug_name, sdl_green ) );
                    }
               }
          }
     }

     std::size_t width = 0;
     for ( const std::pair<std::string,SDL_Color> &tooltip_line : lines ) {
          if ( width < tooltip_line.first.size() * 7 ) {
               width = tooltip_line.first.size() * 7;
          }
     }

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