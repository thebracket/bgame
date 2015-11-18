#pragma once

#include "window.h"
#include "../world/world.h"
#include "line_buffer.h"
#include "colors.h"
#include "../game.h"

namespace render {

class structure_info_window : public window {
public:
     structure_info_window ( sdl2_backend* sdl, const string& title, const bool& decor, const int &entity ) : window ( sdl,title,decor ), entity_id ( entity ) {
          line_buffer lines;

          debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component> ( entity_id );
          if ( name != nullptr ) {
               window_title = name->debug_name;

               description_component * desc = game_engine->ecs->find_entity_component<description_component> ( entity_id );
               if ( desc != nullptr ) {
                    lines.add_line ( SDL, desc->desc, sdl_white );
               }

               // Does it hold anything? If so - add that as well
               vector<item_storage_component *> stored_items = game_engine->ecs->find_components_by_func<item_storage_component> (
               [this] ( const item_storage_component &e ) {
                    if ( e.container_id == entity_id ) {
                         return true;
                    } else {
                         return false;
                    }
               } );
               for ( item_storage_component * item : stored_items ) {
                    debug_name_component * nc = game_engine->ecs->find_entity_component<debug_name_component> ( item->entity_id );
                    lines.add_line ( SDL, string ( " " ) + nc->debug_name, sdl_green );
                    desc = game_engine->ecs->find_entity_component<description_component> ( item->entity_id );
                    if ( desc != nullptr ) {
                         lines.add_line ( SDL, string ( "  " ) + desc->desc, sdl_white );
                    }
               }
          }

          const int height = ( lines.size() *16 ) +24;
          location = { 0, 0, lines.get_width() +32, height };

          allocate();
          render_decorations();

          SDL->texture_target ( texture_id );

          const int x = 16;
          int y = 16;
          for ( const std::pair<std::string,SDL_Color> &panel_line : lines.lines ) {
               std::string line_s = SDL->render_text_to_image ( "disco14", panel_line.first, "tmp", panel_line.second );
               SDL->render_bitmap_simple ( line_s, x, y );
               y += 16;
          }

          SDL->reset_texture_target();
     }

private:
     const int entity_id;
};

}
