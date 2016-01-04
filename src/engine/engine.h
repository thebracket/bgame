#pragma once

#include <stack>
#include <memory>
#include <thread>
#include <functional>

#include "base_mode.hpp"
#include "rng.h"
#include "virtual_terminal.h"
#include "colors.h"
#include "gui/gui.h"
#include "command_manager.h"
#include "sdl2_backend.h"
#include "ecs.hpp"
#include "scene_graph.h"

namespace engine {

template <typename ...Ts> using component_list_t = ecs_detail::type_list<Ts...>;
template <typename ...Ts> using message_list_t = ecs_detail::type_list<Ts...>;

template <typename component_list, typename message_list> 
class bracket_engine {
public:
     bracket_engine(const function<void(fstream &, const int &)> loader, function<void(fstream &)> world_loader, function<void(fstream &)> world_saver) noexcept
     {
	ecs = new entity_component_system<typename component_list::type_list>(loader, world_loader, world_saver);
	messaging = new message_bus<typename message_list::type_list>();
     }
     
     ~bracket_engine() {
	delete ecs;
	delete messaging;
     }
    
     entity_component_system<typename component_list::type_list> * ecs;
     message_bus<typename message_list::type_list> * messaging;
     random_number_generator rng;
          
private:     
     sdl2_backend backend_driver;
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
     void init(const std::string &window_title, const int width=1024, const int height=768) {
          init_virtual_terminal();
          backend_driver.init(window_title, width, height);
          std::pair<int,int> size = backend_driver.get_console_size();
          vterm::resize ( size.first, size.second );
     }

     void main_loop ( unique_ptr<base_mode> starting_mode ) {
          current_mode = std::move ( starting_mode );
          current_mode->init();

          double duration_ms = 0;
          while ( !quitting ) {
               clock_t start_time = clock();
	       messaging->clear();
	       backend_driver.resource_cleanup_tick();
               command::clear_commands();
               backend_driver.poll_inputs();
               ecs->tick ( duration_ms );

               // Render Control
               vterm::clear_screen();

               pair<return_mode, unique_ptr<base_mode>> continuation_mode = current_mode->tick ( duration_ms );
               if ( continuation_mode.first == POP ) pop_mode();
               if ( continuation_mode.first == PUSH ) {
                    continuation_mode.second->init();
                    push_mode ( std::move ( continuation_mode.second ) );
               }
  
	       scene_graph * render_target = current_mode->get_render_target();
	       if (render_target == nullptr) {
		  backend_driver.draw_vterm ( vterm::get_virtual_screen() );
	       } else {
		  render_target->render(&backend_driver);
	       }

               duration_ms = ( ( clock() - start_time ) *1000.0 ) / CLOCKS_PER_SEC;
               if ( fixed_time_step && duration_ms < 33 ) {
                    const int sleepy_time = 33 - duration_ms;
                    std::this_thread::sleep_for ( std::chrono::milliseconds ( sleepy_time ) );
                    duration_ms = 33;
               }
          }
          backend_driver.stop();
     }
     
     void load_image_resource(const std::string &filename, const std::string &tag) {
	  backend_driver.load_image_resource(filename, tag);
     }

     void load_font_resource(const std::string &filename, const std::string &tag, const int &size) {
	  backend_driver.load_font_resource(filename, tag, size);
     }
     
     std::string render_text_to_image(const std::string &font_tag, const std::string text, const std::string &new_tag, SDL_Color color) {
	  return backend_driver.render_text_to_image(font_tag, text, new_tag, color);
     }
};

}
