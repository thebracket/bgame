#include "sdl2_backend.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "command_manager.h"
#include <iostream>

using std::make_pair;

namespace engine {

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;
const int TERMINAL_SIZE = 16;

sdl2_backend::sdl2_backend()
{
     initialized = false;
}

sdl2_backend::~sdl2_backend()
{
     stop();
}

void sdl2_backend::load_image_resource(const std::string &filename, const std::string &tag)
{
    resources.load_image(renderer, filename, tag);
}

void sdl2_backend::load_font_resource(const string& filename, const string& tag, const int& size)
{
    resources.load_font(filename, tag, size);
}

string sdl2_backend::render_text_to_image( const string& font_tag, const string text, const string& new_tag, SDL_Color color )
{
    return resources.render_text_to_texture( renderer, font_tag, text, new_tag, color );
}

void sdl2_backend::init(const std::string &window_title, const int width=1024, const int height=768)
{
     SCREEN_HEIGHT = height;
     SCREEN_WIDTH = width;
     const int error_code = SDL_Init ( SDL_INIT_VIDEO );
     if ( error_code < 0 ) throw 101; // TODO: Real exception

     window = SDL_CreateWindow ( window_title.c_str(),
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                               );
     if ( window == NULL ) throw 102;

     renderer = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
     if ( renderer == NULL ) throw 103;
     
     if ( TTF_Init() == -1 ) throw 104;

     resources.load_image( renderer, "../assets/terminal16x16.png", "font" );

     initialized = true;
}

void sdl2_backend::stop()
{
     if ( initialized ) {
	  resources.clear();
          //SDL_DestroyTexture ( font_image );
          SDL_DestroyRenderer ( renderer );
          SDL_DestroyWindow ( window );
          SDL_Quit();
     }
}

pair< int, int > sdl2_backend::get_console_size()
{
     return make_pair ( SCREEN_WIDTH/TERMINAL_SIZE,SCREEN_HEIGHT/TERMINAL_SIZE );
}

void sdl2_backend::clear_screen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void sdl2_backend::present()
{
    SDL_RenderPresent(renderer);
}

pair< int, int > sdl2_backend::get_screen_size()
{
    return make_pair ( SCREEN_WIDTH, SCREEN_HEIGHT );
}

void sdl2_backend::render_bitmap(const string& tag, const SDL_Rect& source, const SDL_Rect& dest)
{
    SDL_Texture * bmp = resources.get_texture_by_tag(tag);
    SDL_RenderCopy(renderer, bmp, &source, &dest);
}

void sdl2_backend::render_bitmap_simple(const string& tag, const int& x, const int& y)
{
    SDL_Texture * bmp = resources.get_texture_by_tag(tag);
    if (bmp == nullptr) throw 101;
    int image_width, image_height;
    SDL_QueryTexture(bmp, NULL, NULL, &image_width, &image_height);
    SDL_Rect target { x, y, image_width, image_height };
    SDL_Rect src { 0, 0, image_width, image_height };
    //std::cout << image_width << " x " << image_height << "\n";
    SDL_RenderCopy(renderer, bmp, &src, &target );
}

void sdl2_backend::render_bitmap_centered ( const string& tag, const int& x, const int& y )
{
    SDL_Texture * bmp = resources.get_texture_by_tag(tag);
    int image_width, image_height;
    SDL_QueryTexture(bmp, NULL, NULL, &image_width, &image_height);
    SDL_Rect target { x - (image_width / 2), y, image_width, image_height };
    SDL_Rect src { 0, 0, image_width, image_height };
    //std::cout << image_width << " x " << image_height << "\n";
    SDL_RenderCopy(renderer, bmp, &src, &target );
}

void sdl2_backend::draw_vterm ( vector< vterm::screen_character >* screen )
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_Texture * font_image = resources.get_texture_by_tag( "font" );
  
  const int ascii_height = SCREEN_HEIGHT/TERMINAL_SIZE;
  const int ascii_width = SCREEN_WIDTH/TERMINAL_SIZE;
  const SDL_Rect background_source{88, 104, TERMINAL_SIZE, TERMINAL_SIZE};
  
  for (int y=0; y<ascii_height; ++y) {
    for (int x=0; x<ascii_width; ++x) {
      const int screen_x = x * TERMINAL_SIZE;
      const int screen_y = y * TERMINAL_SIZE;
      const unsigned char target_char = screen->operator[]((y*ascii_width)+x).character;
      const tuple<unsigned char, unsigned char, unsigned char> foreground = screen->operator[]((y*ascii_width)+x).foreground_color;
      const tuple<unsigned char, unsigned char, unsigned char> background = screen->operator[]((y*ascii_width)+x).background_color;
      const int texture_x = (target_char % 16) * TERMINAL_SIZE;
      const int texture_y = (target_char / 16) * TERMINAL_SIZE;

      // Where it goes
      SDL_Rect dst_rect{screen_x, screen_y, TERMINAL_SIZE, TERMINAL_SIZE};
      
      // Blit the background
      SDL_SetTextureColorMod(font_image, std::get<0>(background), std::get<1>(background), std::get<2>(background));
      SDL_RenderCopy(renderer, font_image, &background_source, &dst_rect);
      
      // Blit the foreground
      SDL_SetTextureColorMod(font_image, std::get<0>(foreground), std::get<1>(foreground), std::get<2>(foreground));
      SDL_Rect src_rect{texture_x, texture_y, TERMINAL_SIZE, TERMINAL_SIZE};
      SDL_RenderCopy(renderer, font_image, &src_rect, &dst_rect);
    }
  }
  
  SDL_RenderPresent(renderer);
}

command::keys translate_keycode(const SDL_Event &e) {
  switch (e.key.keysym.sym) {
    case SDLK_UP : return command::UP;
    case SDLK_DOWN : return command::DOWN;
    case SDLK_LEFT : return command::LEFT;
    case SDLK_RIGHT : return command::RIGHT;
    case SDLK_RETURN : return command::ENTER;
    case SDLK_q : return command::Q;
    case SDLK_SPACE : return command::SPACE;
  }
  return command::NONE;
}

void sdl2_backend::poll_inputs()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_KEYDOWN) {
	command::on_command({ translate_keycode(e), 0, 0, command::KEYDOWN });
      }
      if (e.type == SDL_MOUSEMOTION) {
	command::on_command({ command::keys::NONE, e.motion.x, e.motion.y, command::MOUSE_MOVE });
      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
	if (e.button.button == SDL_BUTTON_LEFT)	command::on_command({ command::keys::NONE, 0, 0, command::MOUSE_LEFT_CLICK } );
	if (e.button.button == SDL_BUTTON_RIGHT) command::on_command({ command::keys::NONE, 0, 0, command::MOUSE_RIGHT_CLICK } );
      }
    }
}

void sdl2_backend::resource_cleanup_tick()
{
    resources.mark_and_sweep();
}

}