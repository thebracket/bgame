#pragma once

/* Convenience header - includes the rest of the engine */

#include <stack>
#include <memory>
#include <thread>

#include "rng.h"
#include "virtual_terminal.h"
#include "colors.h"
#include "base_mode.h"
#include "gui/gui.h"
#include "backends/command_manager.h"
#include "backends/ansi_backend.h"
#include "backends/ncurses_backend.h"
#include "backends/sdl2_backend.h"
#include "globals.h"

namespace engine {

template <typename backend_t>
class bracket_engine {
private:
     backend_t backend_driver;
     std::unique_ptr<base_mode> current_mode;
     std::stack<std::unique_ptr<base_mode>> mode_stack;
     bool quitting = false;
     const bool fixed_time_step = false;

     void push_mode ( std::unique_ptr< base_mode > new_mode ) {
          current_mode->on_push();
          mode_stack.push ( std::move ( current_mode ) );
          current_mode = std::move ( new_mode );
          current_mode->on_pop();
     }

     void pop_mode() {
          current_mode->done();
          if ( mode_stack.empty() ) {
               quitting = true;
          } else {
               current_mode = std::move ( mode_stack.top() );
               mode_stack.pop();
               current_mode->on_pop();
          }
     }
public:
     void init() {
          init_rng();
          init_virtual_terminal();
          backend_driver.init();
          std::pair<int,int> size = backend_driver.get_console_size();
          vterm::resize ( size.first, size.second );
     }

     void main_loop ( unique_ptr<base_mode> starting_mode ) {
          current_mode = std::move ( starting_mode );
          current_mode->init();

          double duration_ms = 0;
          while ( !quitting ) {
               clock_t start_time = clock();
               command::clear_commands();
               backend_driver.poll_inputs();
               command::process_commands();
               engine::globals::ecs->tick ( duration_ms );

               // Render Control
               vterm::clear_screen();

               pair<return_mode, unique_ptr<base_mode>> continuation_mode = current_mode->tick ( duration_ms );
               if ( continuation_mode.first == POP ) pop_mode();
               if ( continuation_mode.first == PUSH ) {
                    continuation_mode.second->init();
                    push_mode ( std::move ( continuation_mode.second ) );
               }


               backend_driver.draw ( vterm::get_virtual_screen() );

               duration_ms = ( ( clock() - start_time ) *1000.0 ) / CLOCKS_PER_SEC;
               if ( fixed_time_step && duration_ms < 33 ) {
                    const int sleepy_time = 33 - duration_ms;
                    std::this_thread::sleep_for ( std::chrono::milliseconds ( sleepy_time ) );
                    duration_ms = 33;
               }
          }
          backend_driver.stop();
     }
};

}
