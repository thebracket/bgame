#pragma once
#include <vector>
#include <string>
#include <utility>
#include <tuple>

using std::vector;
using std::string;
using std::tuple;
using std::pair;

namespace engine {

namespace vterm {

typedef tuple<unsigned char, unsigned char, unsigned char> color_t; 
  
/* Represents a character on the virtual terminal */
struct screen_character {
    unsigned char character;
    color_t foreground_color;
    color_t background_color;
};

/* Returns the screen size, width/height format */
pair<int,int> get_screen_size();

/* Clears the screen to black, traditional CLS */
void clear_screen();

/* Set a single character on the screen */
void set_char_xy ( const int x, const int y, const engine::vterm::screen_character &c );

/* Adds a simple string to the terminal. No wrapping or other niceties! */
void print(const int x, const int y, const string text, const color_t fg, color_t bg);

/* Draws a single-line box */
void draw_box ( const int &x, const int &y, const int &w, const int &h, const color_t fg, const color_t bg );

/* Draws a double-line box */
void draw_dbl_box ( const int &x, const int &y, const int &w, const int &h, const color_t fg, const color_t bg );

/* Adjusts the buffer size; should be called when a back-end detects a
 * new window size.
 */
void resize(const int new_width, const int new_height);

/* Obtains a pointer to the virtual screen. While this could be used to
 * modify it, it isn't really recommended to do so; we're trying to build
 * libraries to do that!
 */
vector<screen_character> * get_virtual_screen();

/*
 * Darkens a color by the specified amount.
 */
inline void darken ( const int &amount, color_t &col )
{
     unsigned char red = std::get<0> ( col );
     unsigned char green = std::get<1> ( col );
     unsigned char blue = std::get<2> ( col );

     if ( red > amount ) {
          red -= amount;
     } else {
          red = 0;
     }
     if ( green > amount ) {
          green -= amount;
     } else {
          green = 0;
     }
     if ( blue > amount ) {
          blue -= amount;
     } else {
          blue = 0;
     }

     std::get<0> ( col ) = red;
     std::get<1> ( col ) = green;
     std::get<2> ( col ) = blue;
}

/*
 * Calculates the luminance of a color, and converts it to grey-scale.
 */
inline void desaturate ( color_t &col )
{
     unsigned char red = std::get<0> ( col );
     unsigned char green = std::get<1> ( col );
     unsigned char blue = std::get<2> ( col );

     // Goes here
     float RED = red/255.0F;
     float GREEN = green/255.0F;
     float BLUE = blue/255.0F;
     float luminance = 0.299 * RED + 0.587 * GREEN + 0.114 * BLUE;
     
     red = (luminance * 255.0F);
     green = ( luminance * 255.0F);
     blue = ( luminance * 255.0F);

     std::get<0> ( col ) = red;
     std::get<1> ( col ) = green;
     std::get<2> ( col ) = blue;
}

}

/* Initializes the virtual terminal system */
void init_virtual_terminal();


}
