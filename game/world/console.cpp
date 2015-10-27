#include "console.h"
#include "../../engine/gui/gui_element_base.h"
#include <map>

// To keep gcc happy, these are defined outside of the map.


std::map<string,color_t> decode_console_color {
     {"@BLACK@", engine::black},
     {"@WHITE@", engine::grey},
     {"@RED@", engine::dark_red},
     {"@GREEN@", engine::dark_green},
     {"@BLUE@", engine::dark_blue},
     {"@YELLOW@", engine::dark_yellow},
     {"@MAGENTA@", engine::dark_magenta},
     {"@CYAN@", engine::dark_cyan},
     {"@B_WHITE@", engine::white},
     {"@B_RED@", engine::red},
     {"@B_GREEN@", engine::green},
     {"@B_BLUE@", engine::blue},
     {"@B_YELLOW@", engine::yellow},
     {"@B_MAGENTA@", engine::magenta},
     {"@B_CYAN@", engine::cyan}
};

void console::write ( const string line )
{
     vector<console_character> parsed;
     color_t current_color = engine::grey;
     bool in_control_code = false;
     string current_control_code = "";

     for ( unsigned int i=0; i<line.size(); ++i ) {
          const unsigned char current_char = line[i];
          if ( in_control_code ) {
               if ( current_char != '@' ) {
                    current_control_code += current_char;
               } else {
		    current_control_code += current_char;
                    in_control_code = false;
                    current_color = decode_console_color[current_control_code];

               }
          } else {
               if ( current_char == '@' ) {
                    current_control_code = "@";
                    in_control_code = true;
               } else {
                    console_character c { current_char, current_color };
                    parsed.push_back ( c );
               }
          }
     }

     if ( log.size() > 100 ) log.erase ( log.begin() );
     log.push_back ( {parsed} );
}
