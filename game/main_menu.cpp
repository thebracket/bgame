#include "main_menu.h"

using std::make_unique;

void main_menu::init()
{
  count = 0;
  
  unique_ptr<gui_frame> frame = make_unique<gui_frame>(FILL_REGION);
  frame->add_child(make_unique<gui_static_text>("Black Future", 1, 1, red, black, true));
  frame->add_child(make_unique<gui_static_text>("One day, I'll grow up and become a real menu!", 1, 4, grey, black, true));
  
  menu_interface.add_child(std::move(frame));
}

void main_menu::done()
{

}

pair< return_mode, unique_ptr< base_mode > > main_menu::tick ( const double time_elapsed )
{
  menu_interface.render();
  
  // For now, we quit after 100 iterations
  ++count;
  if (count > 100) {
    return make_pair ( POP, NO_PUSHED_MODE );
  } else {
    return make_pair ( CONTINUE, NO_PUSHED_MODE );
  }
}
