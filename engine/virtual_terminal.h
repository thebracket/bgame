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

}

/* Initializes the virtual terminal system */
void init_virtual_terminal();

}
