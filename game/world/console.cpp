#include "console.h"
#include <map>

std::map<string,color_t> decode_console_color {
     {"@BLACK@", {0,0,0}},
     {"@WHITE@", {128,128,128}},
     {"@RED@", {128,0,0}},
     {"@GREEN@", {0,128,0}},
     {"@BLUE@", {0,0,128}},
     {"@YELLOW@", {128,128,0}},
     {"@MAGENTA@", {128,0,128}},
     {"@CYAN@", {0,128,128}},

     {"@B_WHITE@", {255,255,255}},
     {"@B_RED@", {255,0,0}},
     {"@B_GREEN@", {0,255,0}},
     {"@B_BLUE@", {0,0,255}},
     {"@B_YELLOW@", {255,255,0}},
     {"@B_MAGENTA@", {255,0,255}},
     {"@B_CYAN@", {0,255,255}}
};

void console::write ( const string line )
{
     vector<console_character> parsed;
     color_t current_color = {128,128,128};
     bool in_control_code = false;
     string current_control_code = "";

     for ( int i=0; i<line.size(); ++i ) {
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
