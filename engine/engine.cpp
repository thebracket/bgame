#include "engine.h"
#include "output_backend.h"
#include "ncurses_backend.h"
#include "sdl2_backend.h"
#include "ecs/ecs.h"
#include <memory>
#include <thread>
#include <chrono>
#include <stack>

using std::unique_ptr;
using std::make_unique;
using std::stack;

namespace engine {

namespace detail {

unique_ptr<output_backend> backend_driver;
unique_ptr<base_mode> current_mode;
stack<unique_ptr<base_mode>> mode_stack;
bool quitting = false;
const bool fixed_time_step = false;

}

using namespace detail;

void push_mode ( unique_ptr<base_mode> new_mode )
{
     current_mode->on_push();
     mode_stack.push ( std::move ( current_mode ) );
     current_mode = std::move ( new_mode );
     current_mode->on_pop();
}

void pop_mode()
{
     current_mode->done();
     if ( mode_stack.empty() ) {
          quitting = true;
     } else {
          current_mode = std::move ( mode_stack.top() );
          mode_stack.pop();
          current_mode->on_pop();
     }
}

void init(backend_mode mode)
{
     init_rng();
     init_virtual_terminal();

     switch (mode) {
       case NCURSES : backend_driver = make_unique<ncurses_backend>(); break;
       case SDL2 : backend_driver = make_unique<sdl2_backend>(); break;
     }
     
     backend_driver->init();
     pair<int,int> size = backend_driver->get_console_size();
     vterm::resize ( size.first, size.second );
}

void main_loop ( unique_ptr<base_mode> starting_mode )
{
     current_mode = std::move ( starting_mode );
     current_mode->init();

     double duration_ms = 0;
     while ( !quitting ) {
          clock_t start_time = clock();
	  command::clear_commands();
          backend_driver->poll_inputs();
	  command::process_commands();
	  ecs::tick(duration_ms);

	  // Render Control
	  vterm::clear_screen();

          pair<return_mode, unique_ptr<base_mode>> continuation_mode = current_mode->tick ( duration_ms );
          if ( continuation_mode.first == POP ) pop_mode();
          if ( continuation_mode.first == PUSH ) {
	       continuation_mode.second->init();
               push_mode ( std::move ( continuation_mode.second ) );
          }

          
          backend_driver->draw ( vterm::get_virtual_screen() );
	  
	  duration_ms = ((clock() - start_time)*1000.0) / CLOCKS_PER_SEC;
	  if (fixed_time_step && duration_ms < 33) {
	    const int sleepy_time = 33 - duration_ms;
	    std::this_thread::sleep_for(std::chrono::milliseconds(sleepy_time));
            duration_ms = 33;
	  }
     }
     backend_driver->stop();
}


}