#include "play_mode.h"

using namespace engine;
using std::make_unique;

void play_mode::init()
{
  ui.add_child(make_unique<gui_frame>("Main", TRI_MAIN));
  ui.add_child(make_unique<gui_frame>("Console", TRI_CONSOLE));
  ui.add_child(make_unique<gui_frame>("Panel", TRI_PANEL));
  count = 0;
}

void play_mode::done()
{

}

pair< return_mode, unique_ptr< base_mode > > play_mode::tick(const double time_elapsed)
{
  ui.render();
  
  ++count;
  if (count < 100) {
    return make_pair ( CONTINUE, NO_PUSHED_MODE );
  }
  return make_pair ( POP, NO_PUSHED_MODE );
}
