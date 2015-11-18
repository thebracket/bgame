#include "window.h"
#include "colors.h"
#include <iostream>

namespace render {

window::~window()
{
     if ( !texture_id.empty() ) {
          SDL->mark_texture_for_deletion ( texture_id );
     }
}

void window::render()
{
     SDL_Rect src { 0, 0, location.w, location.w };
     SDL->render_bitmap ( texture_id, src, location );
}

void window::allocate()
{
     texture_id = SDL->create_temporary_texture ( location.w, location.h );
}

void window::render_decorations()
{
     SDL->texture_target ( texture_id );
     SDL->clear_screen();

     SDL_Rect source = raws::get_tile_source_by_name ( "WINDOWMASK" );
     SDL_Rect dest { 0, 0, location.w, location.h };
     SDL->render_bitmap ( "spritesheet", source, dest );

     if ( render_decor ) {
          source = raws::get_tile_source_by_name ( "WINDOWHEAD" );
          dest = { 0, 0, location.w, 16 };
          SDL->render_bitmap ( "spritesheet", source, dest );

          const string title_texture = SDL->render_text_to_image ( "disco14", window_title, "tmp", sdl_white );
          SDL->render_bitmap_centered ( title_texture, location.x + ( location.w/2 ), location.y + 1 );
     }

     SDL->reset_texture_target ();
}

}