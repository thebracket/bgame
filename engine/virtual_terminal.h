#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace engine {

namespace vterm {

/* Represents a character on the virtual terminal */
struct screen_character {
    unsigned char character;
};

/* Clears the screen to black, traditional CLS */
void clear_screen();

/* Adds a simple string to the terminal. No wrapping or other niceties! */
void print(int x, int y, string text);

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