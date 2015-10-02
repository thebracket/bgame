#include "sdl2_backend.h"
#include <SDL2/SDL_image.h>

using std::make_pair;

namespace engine {

// TODO: Configuration driven height
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

sdl2_backend::sdl2_backend()
{
     initialized = false;
}

sdl2_backend::~sdl2_backend()
{
     stop();
}

void sdl2_backend::init()
{
     const int error_code = SDL_Init ( SDL_INIT_VIDEO );
     if ( error_code < 0 ) throw 101; // TODO: Real exception

     // TODO: Configuration driven window title
     window = SDL_CreateWindow ( "Bert's Silly Game",
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                               );
     if ( window == NULL ) throw 102;

     renderer = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
     if ( renderer == NULL ) throw 103;

     int image_flags = IMG_INIT_PNG;
     if ( ! ( IMG_Init ( image_flags ) & image_flags ) ) throw 104;

     SDL_Surface * font_surface = IMG_Load ( "../assets/terminal8x8_palette2.png" );
     if ( font_surface == NULL ) throw 104;
     font_image = SDL_CreateTextureFromSurface ( renderer, font_surface );
     SDL_FreeSurface ( font_surface );


     initialized = true;
}

void sdl2_backend::stop()
{
     if ( initialized ) {
          SDL_DestroyTexture ( font_image );
          SDL_DestroyRenderer ( renderer );
          SDL_DestroyWindow ( window );
          SDL_Quit();
     }
}

pair< int, int > sdl2_backend::get_console_size()
{
     return make_pair ( SCREEN_WIDTH/8,SCREEN_HEIGHT/8 );
}

void sdl2_backend::draw ( vector< vterm::screen_character >* screen )
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  const int ascii_height = SCREEN_HEIGHT/8;
  const int ascii_width = SCREEN_WIDTH/8;
  
  for (int y=0; y<ascii_height; ++y) {
    for (int x=0; x<ascii_width; ++x) {
      const int screen_x = x * 8;
      const int screen_y = y * 8;
      const unsigned char target_char = screen->operator[]((y*ascii_width)+x).character;
      const int texture_x = (target_char % 16) * 8;
      const int texture_y = (target_char / 16) * 8;
      
      SDL_Rect dst_rect{screen_x, screen_y, 8, 8};
      SDL_Rect src_rect{texture_x, texture_y, 8, 8};
      SDL_RenderCopy(renderer, font_image, &src_rect, &dst_rect);
    }
  }
  
  SDL_RenderPresent(renderer);
}


}