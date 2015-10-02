#include "virtual_terminal.h"
#include <algorithm>
#include <memory>

using std::unique_ptr;
using std::make_unique;
using std::fill;

namespace engine {

namespace vterm {

const screen_character blank_character {' '};

int screen_width;
int screen_height;
int n_chars;
unique_ptr<vector<screen_character>> terminal_buffer;

inline int screen_idx(const int x, const int y) {
  return (y*screen_width)+x;
}

void clear_screen() {
    fill(terminal_buffer->begin(), terminal_buffer->end(), blank_character);
}

void print(int x, int y, string text)
{  
  int idx = screen_idx(x,y);
  for (int i=0; i<text.size(); ++i) {
    terminal_buffer->operator[](idx+i).character = text[i];
  }
}


void resize(const int new_width, const int new_height) {
    screen_width = new_width;
    screen_height = new_height;
    n_chars = screen_width * screen_height;
    terminal_buffer->resize(n_chars);
}

vector<screen_character> * get_virtual_screen() {
    return terminal_buffer.get();
}
} // end vterm namespace

using namespace vterm;

void init_virtual_terminal()
{
    // Temporary
    terminal_buffer = make_unique<vector<screen_character>>();
    resize(80,25);
    clear_screen();
}


}
