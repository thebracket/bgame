#include "world_gen_mode.h"
#include "world/worldgen.h"
#include <mutex>
#include <string>

using std::make_unique;
using std::thread;
using std::lock_guard;
using std::mutex;
using std::string;

void world_gen_mode::init()
{
     unique_ptr<gui_frame> frame = make_unique<gui_frame> ( "Outer", FILL_REGION );
     frame->add_child ( make_unique<gui_static_text> ( "Title", "Terragenesis", 1, 1, red, black, true ) );
     frame->add_child ( make_unique<gui_static_text> ( "Quote", "Please wait while the Magratheans assemble your planet...", 1, 2, grey, black, true ) );
     ui.add_child ( std::move ( frame ) );

     lock_guard<mutex> lock ( worldgen::worldgen_mutex );
     worldgen::progress = worldgen::INIT;
     worldgen::heightmap_progress = nullptr;
     worldgen::current_landblock = nullptr;
     world_gen_thead = make_unique<thread> ( worldgen::build_world );
}

void world_gen_mode::done()
{

}

pair< return_mode, unique_ptr< base_mode > > world_gen_mode::tick ( const double time_elapsed )
{
     ui.render();

     // Using a local scope to ensure that the mutex isn't locked for long
     {
          lock_guard<mutex> lock ( worldgen::worldgen_mutex );
          switch ( worldgen::progress ) {
          case worldgen::INIT :
               render_init();
               break;
          case worldgen::FAULTLINES :
               render_faultlines();
               break;
          case worldgen::MAKETILES :
               render_tiles();
               break;
          case worldgen::DONE :
               render_done();
               break;
          }
     }

     if ( worldgen::progress == worldgen::DONE and command::is_key_down ( command::ENTER ) ) {
          return make_pair ( POP, NO_PUSHED_MODE ); // Back to menu
     }

     // Keep going
     return make_pair ( CONTINUE, NO_PUSHED_MODE );
}

void world_gen_mode::render_init()
{
     const string init_message = "And He saw that it was good, zeroed memory, and generally tidied up.";
     const std::pair<int,int> size = engine::vterm::get_screen_size();
     const int y = size.second/2;
     const int x = ( size.first / 2 ) - init_message.size() /2;
     engine::vterm::print ( x,y,init_message,white,black );
}

void world_gen_mode::render_faultlines()
{
     const int lx = 4;
     const int ty = 4;

     for ( int y=0; y<world::world_height; ++y ) {
          for ( int x=0; x<world::world_width; ++x ) {
               const int screen_x = lx + x;
               const int screen_y = ty + y;
               color_t tile_color {0, 128+worldgen::heightmap_progress->operator[] ( world::world_idx ( x,y ) ), 0};
               engine::vterm::screen_character sc { 219, tile_color, tile_color };
               vterm::set_char_xy ( screen_x, screen_y, sc );
          }
     }
}

void world_gen_mode::render_tiles()
{
     if ( worldgen::current_landblock == nullptr ) return;

     const std::pair<int,int> size = engine::vterm::get_screen_size();
     const int lx = 4;
     const int ty = 4;
     for ( int y=0; y<size.second-8; ++y ) {
          for ( int x=0; x<size.first-8; ++x ) {
               const int index = worldgen::current_landblock->idx ( x,y );
               const int screen_x = lx + x;
               const int screen_y = ty + y;
	       const unsigned char glyph = worldgen::current_landblock->tiles[index].display;
	       const color_t fg = worldgen::current_landblock->tiles[index].foreground;
	       const color_t bg = worldgen::current_landblock->tiles[index].background;
	       engine::vterm::set_char_xy ( screen_x, screen_y, {glyph, fg, bg} );
          }
     }
}


void world_gen_mode::render_done()
{
     if ( worldgen::heightmap_progress != nullptr ) {
          worldgen::heightmap_progress = nullptr;
     }

     const string init_message = "Your world is ready to use, press ENTER.";
     const std::pair<int,int> size = engine::vterm::get_screen_size();
     const int y = size.second/2;
     const int x = ( size.first / 2 ) - init_message.size() /2;
     engine::vterm::print ( x,y,init_message,white,black );
}


