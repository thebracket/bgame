#include "main_menu.h"

void main_menu::init()
{

}

void main_menu::done()
{

}

pair< return_mode, unique_ptr< base_mode > > main_menu::tick ( const double time_elapsed )
{
  // Quit immediately
  return make_pair ( POP, NO_PUSHED_MODE );
}
