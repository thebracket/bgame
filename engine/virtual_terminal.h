#pragma once
#include <vector>
#include <string>
#include <utility>

using std::vector;
using std::string;
using std::tuple;

namespace engine {

namespace vterm {

/* Represents a character on the virtual terminal */
struct screen_character {
    unsigned char character;
    tuple<unsigned char, unsigned char, unsigned char> foreground_color;
    tuple<unsigned char, unsigned char, unsigned char> background_color;
};

/* Clears the screen to black, traditional CLS */
void clear_screen();

/* Adds a simple string to the terminal. No wrapping or other niceties! */
void print(const int x, const int y, const string text, const tuple<unsigned char, unsigned char, unsigned char> fg, const tuple<unsigned char, unsigned char, unsigned char> bg);

/* Draws a double-line box */
void draw_dbl_box ( const int &x, const int &y, const int &w, const int &h, const tuple<unsigned char, unsigned char, unsigned char> fg, const tuple<unsigned char, unsigned char, unsigned char> bg );

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